#include "db_manager.hpp"
#include "db_utils.hpp"

DatabaseManager::DatabaseManager()
: conn_{DB_STRING}
{
  try {
    prepare_statements();
  } catch (const pqxx::pqxx_exception &e) {
    std::cerr << e.base().what();
  }
}

auto DatabaseManager::insert_user(std::string username, std::string email, std::string password) -> bool {
  return execute0("insert_user", username, email, hash_password(password));
}

auto DatabaseManager::get_user(std::string email) -> User* {
  auto res = execute("email_get_user", email);
  if (res.size() < 1) {
    return NULL;
  }
  return new User(res[0]);
}

auto DatabaseManager::get_user(int id) -> User* {
  auto res = execute("id_get_user", id);
  if (res.size() != 1) {
    return NULL;
  }
  return new User(res[0]);
}

auto DatabaseManager::get_user_username(std::string username) -> User* {
  auto res = execute("username_get_user", username);
  if (res.size() != 1) {
    return NULL;
  }
  return new User(res[0]);
}

auto DatabaseManager::save_match(std::string gameType, bool is_ranked, std::map<int, int> playersELO, int winner,
    std::string potholes, std::string move_seq, std::string svg_data, std::vector<uint64_t> snapshots) -> int {
  auto playerOutcomes = std::map<int, std::string>{};
  for (auto const& entry : playersELO) {
    auto player = entry.first;
    auto outcome = (playersELO.count(winner) == 0) ? "DRAW" : (winner == player) ? "WIN" : "LOSS";
    playerOutcomes.insert({player, outcome});
  }
  return save_match(gameType, is_ranked, playersELO, playerOutcomes, potholes, move_seq, svg_data, snapshots);
}

auto DatabaseManager::save_match(std::string gameType, bool is_ranked, std::map<int, int> playersELO, std::map<int, std::string> playerOutcomes,
    std::string potholes, std::string move_seq, std::string svg_data, std::vector<uint64_t> snapshots) -> int {
  // Insert Match
  auto res = execute("insert_match", gameType, is_ranked, potholes, move_seq, svg_data);
  auto matchID = atoi(res[0][0].c_str());
  // Insert Players & Outcomes
  auto outcome_columns = std::vector<std::string>{"player", "match", "end_elo", "outcome"};
  auto outcome_entries = std::vector<std::tuple<int, int, int, std::string>>{};
  for (auto const& entry : playersELO) {
    auto player = entry.first;
    auto end_elo = entry.second;
    auto outcome = playerOutcomes[player];
    outcome_entries.push_back(std::make_tuple(player, matchID, end_elo, outcome));
  }
  batch_insert("outcomes", outcome_columns, outcome_entries);
  // Insert Snapshots
  auto snapshot_columns = std::vector<std::string>{"match", "move_num", "boardstate"};
  auto snapshot_entries = std::vector<std::tuple<int, int, uint64_t>>{};
  auto move_num = 1;
  for (const auto &snapshot : snapshots) {
    snapshot_entries.push_back(std::make_tuple(matchID, move_num, snapshot));
    move_num++;
  }
  batch_insert("snapshots", snapshot_columns, snapshot_entries);
  return matchID;
}

auto DatabaseManager::get_match(int id) -> Match* {
  auto res = execute("get_match", id);
  if (res.size() < 1) {
    return nullptr;
  }
  auto match = atoi(res[0][0].c_str());
  auto game = res[0][1].c_str();
  auto ranked = std::string(res[0][2].c_str()) == "t";
  auto potholes = res[0][3].c_str();
  auto replay = res[0][4].c_str();
  auto svg_data = res[0][5].c_str();
  auto curmatch = new Match(match, game, ranked, potholes, replay, svg_data, std::vector<Player>{});
  for (auto const &row : res) {
    // Add player
    auto p_id = atoi(row[6].c_str());
    auto p_username = row[7].c_str();
    auto p_startelo = atoi(row[8].c_str());
    auto p_endelo = atoi(row[9].c_str());
    auto p_outcome = row[10].c_str();
    auto *player = new Player(p_id, p_username, p_startelo, p_endelo, p_outcome);
    curmatch->players.push_back(*player);
  }
  return curmatch;
}

auto DatabaseManager::get_matches() -> std::vector<Match*> {
  auto res = execute("get_matches");
  return parse_matches(res);
}

auto DatabaseManager::get_matches(int id) -> std::vector<Match*> {
  auto res = execute("get_matches_user", id);
  return parse_matches(res);
}

auto DatabaseManager::get_last_match(int id) -> Match* {
  auto matches = get_matches(id);
  return (matches.empty()) ? nullptr : matches.back();
}

auto DatabaseManager::get_matches(int move_n, int64_t bs) -> std::vector<Match*> {
  auto res = execute("get_matches_snapshot1", move_n, bs);
  return parse_matches(res);
}

auto DatabaseManager::get_matches(int move_n1, int64_t bs1, int move_n2, int64_t bs2) -> std::vector<Match*> {
  auto res = execute("get_matches_snapshot2", move_n1, bs1, move_n2, bs2);
  return parse_matches(res);
}

auto DatabaseManager::get_latest_elo(int id, std::string gameType) -> int {
  auto res = execute("get_latest_elo", id, gameType);
  if (res.size() < 1) {
    return 0;
  }
  return atoi(res[0][0].c_str());
}

auto DatabaseManager::get_stats(int id) -> PlayerStats* {
  auto res = execute("get_stats", id);
  auto stats = std::map<std::string, int>{};
  for (auto const &row : res) {
    auto game = row[0].c_str();
    auto ranked = (std::string(row[1].c_str()) == "t");
    auto outcome = row[2].c_str();
    auto key = std::string(game).append(ranked ? " RANKED " : " UNRANKED ").append(outcome);
    auto count = atoi(row[3].c_str());
    stats.insert({key, count});
  }
  auto result = new PlayerStats(stats);
  return result;
}

auto DatabaseManager::get_elo_progress(int id, std::string mode) -> std::vector<int> {
  auto res = execute("get_elo_progress", id, mode);
  auto progress = std::vector<int>{1000};
  for (auto const &row : res) {
    auto elo = atoi(row[0].c_str());
    progress.push_back(elo);
  }
  return progress;
}

auto DatabaseManager::get_global_rank(std::string gameType, int id) -> int {
  auto entries = get_global_leaderboard(gameType);
  auto rank = 1;
  for (auto const &e : entries) {
    if (e.uid == id) {
      return rank;
    }
    rank++;
  }
  return -1;
}

auto DatabaseManager::get_global_leaderboard(std::string gameType) -> std::vector<LeaderboardEntry> {
  auto res = execute("get_global_leaderboard", gameType);
  auto leaderboard = std::vector<LeaderboardEntry>{};
  auto rank = 1;
  for (auto const &row : res) {
    auto uid = atoi(row[0].c_str());
    auto username = row[1].c_str();
    auto elo = atoi(row[2].c_str());
    auto wins = atoi(row[3].c_str());
    auto losses = atoi(row[4].c_str());
    auto draws = atoi(row[5].c_str());
    auto entry = new LeaderboardEntry(rank, uid, username, elo, wins, losses, draws);
    leaderboard.push_back(*entry);
    rank++;
  }
  return leaderboard;
}

auto DatabaseManager::get_friend_leaderboard(std::string gameType, int id) -> std::vector<LeaderboardEntry> {
  auto res = execute("get_friend_leaderboard", gameType, id);
  auto leaderboard = std::vector<LeaderboardEntry>{};
  auto rank = 1;
  for (auto const &row : res) {
    auto uid = atoi(row[0].c_str());
    auto username = row[1].c_str();
    auto elo = atoi(row[2].c_str());
    auto wins = atoi(row[3].c_str());
    auto losses = atoi(row[4].c_str());
    auto draws = atoi(row[5].c_str());
    auto entry = new LeaderboardEntry(rank, uid, username, elo, wins, losses, draws);
    leaderboard.push_back(*entry);
    rank++;
  }
  return leaderboard;
}

auto DatabaseManager::get_friends(int id) -> std::vector<User*> {
  auto friends = std::vector<User*>();
  auto res = execute("get_friends_uid", id);
  for (auto row : res) {
    if (atoi(row[0].c_str()) != id) {
      friends.push_back(get_user(atoi(row[0].c_str())));
    }
    if (atoi(row[1].c_str()) != id) {
      friends.push_back(get_user(atoi(row[1].c_str())));
    }
  }
  return friends;
}

auto DatabaseManager::are_friends(int id1, int id2) -> bool {
  auto res = execute("are_friends", id1, id2);
  return res.size() != 0;
}

auto DatabaseManager::get_incoming_freqs(int id) -> std::vector<User*> {
  auto incoming = std::vector<User*>();
  auto res = execute("get_incoming_freqs", id);
  for (auto row : res) {
    incoming.push_back(get_user(atoi(row[0].c_str())));
  }
  return incoming;
}

auto DatabaseManager::get_outgoing_freqs(int id) -> std::vector<User*> {
  auto outgoing = std::vector<User*>();
  auto res = execute("get_outgoing_freqs", id);
  for (auto row : res) {
    outgoing.push_back(get_user(atoi(row[0].c_str())));
  }
  return outgoing;
}

auto DatabaseManager::delete_friend(int from, int to) -> bool {
  auto fri1 = std::min(from, to);
  auto fri2 = std::max(from, to);
  return execute0("delete_friend", fri1, fri2);
}

auto DatabaseManager::send_friend_req(int from, int to) -> bool {
  if (from == to) {
    return false;
  }
  return execute0("send_friend_req", from, to);
}

auto DatabaseManager::accept_friend_req(int accepter, int accepted) -> bool {
  auto fri1 = std::min(accepter, accepted);
  auto fri2 = std::max(accepter, accepted);
  bool remove = execute0("delete_friend_req", accepted, accepter);
  bool add = execute0("add_friend", fri1, fri2);
  return add && remove;
}

auto DatabaseManager::deny_friend_req(int denier, int denied) -> bool {
  return execute0("delete_friend_req", denied, denier);
}

auto DatabaseManager::revoke_friend_req(int revoker, int revoked) -> bool {
  return execute0("delete_friend_req", revoker, revoked);
}

auto DatabaseManager::insert_varification_code(int user, std::string var_code) -> bool {
  return execute0("insert_var_code", user, var_code);
}

auto DatabaseManager::change_password(int user, std::string new_password) -> bool {
  return execute0("change_password", new_password, user);
}

// Prepare the statements on instantiation.
auto DatabaseManager::prepare_statements() -> void {
  conn_.prepare("insert_user",
  "INSERT INTO users (username, email, password_hash) "
  "VALUES ($1, $2, $3);");
  conn_.prepare("email_get_user",
  "SELECT * FROM users "
  "WHERE email = $1;");
  conn_.prepare("id_get_user",
  "SELECT * FROM users "
  "WHERE id = $1;");
  conn_.prepare("username_get_user",
  "SELECT * FROM users "
  "WHERE username = $1;");
  conn_.prepare("insert_match",
  "INSERT INTO matches(game, ranked, potholes, replay, svg_data) "
  "VALUES ($1, $2, $3, $4, $5) "
  "RETURNING id;");
  conn_.prepare("get_match",
  "SELECT matches.id, game, ranked, potholes, replay, svg_data, outcomes.player, "
  "username, "
  "COALESCE("
  "(SELECT o.end_elo FROM matches m "
  "JOIN outcomes o ON m.id = o.match "
  "WHERE m.game = matches.game AND "
  "m.ranked = matches.ranked AND "
  "o.player = outcomes.player AND "
  "m.id < matches.id "
  "ORDER BY m.id "
  "DESC LIMIT 1), 1000) "
  "AS start_elo, "
  "end_elo, outcome "
  "FROM matches "
  "JOIN outcomes ON matches.id = outcomes.match "
  "JOIN users ON outcomes.player = users.id "
  "WHERE matches.id = $1;");
  // TODO: Refactor the two below - similar SQL.
  conn_.prepare("get_matches",
  "SELECT matches.id, game, ranked, potholes, replay, svg_data, outcomes.player, "
  "username, "
  "COALESCE("
  "(SELECT o.end_elo FROM matches m "
  "JOIN outcomes o ON m.id = o.match "
  "WHERE m.game = matches.game AND "
  "m.ranked = matches.ranked AND "
  "o.player = outcomes.player AND "
  "m.id < matches.id "
  "ORDER BY m.id "
  "DESC LIMIT 1), 1000) "
  "AS start_elo, "
  "end_elo, outcome "
  "FROM matches "
  "JOIN outcomes ON matches.id = outcomes.match "
  "JOIN users ON outcomes.player = users.id;");
  conn_.prepare("get_matches_user",
  "SELECT matches.id, game, ranked, potholes, replay, svg_data, outcomes.player, "
  "username, "
  "COALESCE("
  "(SELECT o.end_elo FROM matches m "
  "JOIN outcomes o ON m.id = o.match "
  "WHERE m.game = matches.game AND "
  "m.ranked = matches.ranked AND "
  "o.player = outcomes.player AND "
  "m.id < matches.id "
  "ORDER BY m.id "
  "DESC LIMIT 1), 1000) "
  "AS start_elo, "
  "end_elo, outcome "
  "FROM matches "
  "JOIN outcomes ON matches.id = outcomes.match "
  "JOIN users ON outcomes.player = users.id "
  "WHERE match IN "
  "(SELECT matches.id FROM matches "
  "JOIN outcomes ON matches.id = outcomes.match "
  "WHERE outcomes.player = $1);");
  conn_.prepare("get_matches_snapshot1",
  "SELECT matches.id, game, ranked, potholes, replay, svg_data, outcomes.player, "
  "username, "
  "COALESCE("
  "(SELECT o.end_elo FROM matches m "
  "JOIN outcomes o ON m.id = o.match "
  "WHERE m.game = matches.game AND "
  "m.ranked = matches.ranked AND "
  "o.player = outcomes.player AND "
  "m.id < matches.id "
  "ORDER BY m.id "
  "DESC LIMIT 1), 1000) "
  "AS start_elo, "
  "end_elo, outcome "
  "FROM matches "
  "JOIN outcomes ON matches.id = outcomes.match "
  "JOIN users ON outcomes.player = users.id "
  "WHERE match IN ("
  "SELECT match FROM snapshots "
  "WHERE move_num = $1 AND boardstate = $2) "
  "ORDER BY id;");
  conn_.prepare("get_matches_snapshot2",
  "WITH s1 AS ("
  "SELECT * FROM snapshots "
  "WHERE move_num = $1 AND boardstate = $2), "
  "s2 AS ("
  "SELECT * FROM snapshots "
  "WHERE move_num = $3 AND boardstate = $4) "
  "SELECT matches.id, game, ranked, potholes, replay, svg_data, outcomes.player, "
  "username, "
  "COALESCE("
  "(SELECT o.end_elo FROM matches m "
  "JOIN outcomes o ON m.id = o.match "
  "WHERE m.game = matches.game AND "
  "m.ranked = matches.ranked AND "
  "o.player = outcomes.player AND "
  "m.id < matches.id "
  "ORDER BY m.id "
  "DESC LIMIT 1), 1000) "
  "AS start_elo, "
  "end_elo, outcome "
  "FROM matches "
  "JOIN outcomes ON matches.id = outcomes.match "
  "JOIN users ON outcomes.player = users.id "
  "WHERE match IN ("
  "SELECT s1.match FROM s1 "
  "JOIN s2 ON s1.match = s2.match) "
  "ORDER BY id;");
  conn_.prepare("get_latest_elo",
  "SELECT COALESCE("
  "(SELECT end_elo "
  "FROM outcomes "
  "JOIN matches ON outcomes.match = matches.id "
  "WHERE player = $1 AND ranked = true "
  "AND game = $2 "
  "ORDER BY matches.id DESC "
  "LIMIT 1), 1000);");
  conn_.prepare("get_stats",
  "SELECT game, ranked, outcome, count(outcome) "
  "FROM outcomes "
  "JOIN matches ON outcomes.match = matches.id "
  "WHERE player = $1 "
  "GROUP BY game, ranked, outcome;");
  conn_.prepare("get_elo_progress",
  "SELECT end_elo "
  "FROM outcomes "
  "JOIN matches ON outcomes.match = matches.id "
  "WHERE player = $1 AND game = $2 AND ranked = true "
  "ORDER BY matches.id");
  conn_.prepare("get_global_leaderboard",
  "SELECT users.id, users.username, COALESCE("
  "(SELECT o.end_elo FROM matches m "
  "JOIN outcomes o ON m.id = o.match "
  "JOIN users u ON o.player = u.id "
  "WHERE u.id = users.id AND m.game = matches.game AND m.ranked = true "
  "ORDER BY m.id DESC "
  "LIMIT 1), 1000) as elo, "
  "(SELECT COUNT(outcome) "
  "FROM matches m "
  "JOIN outcomes o ON m.id = o.match "
  "WHERE o.player = users.id "
  "AND outcome = 'WIN' AND m.ranked = true "
  "AND m.game = matches.game) "
  "as wins, "
  "(SELECT COUNT(outcome) "
  "FROM matches m "
  "JOIN outcomes o ON m.id = o.match "
  "WHERE o.player = users.id "
  "AND outcome = 'LOSS' AND m.ranked = true "
  "AND m.game = matches.game) "
  "as losses, "
  "(SELECT COUNT(outcome) "
  "FROM matches m "
  "JOIN outcomes o ON m.id = o.match "
  "WHERE o.player = users.id "
  "AND outcome = 'DRAW' AND m.ranked = true "
  "AND m.game = matches.game) "
  "as draws "
  "FROM users "
  "JOIN outcomes ON outcomes.player = users.id "
  "JOIN matches ON outcomes.match = matches.id "
  "WHERE game = $1 "
  "AND users.id NOT IN (6, 7, 8) "
  "GROUP BY game, users.id "
  "ORDER BY elo DESC, username;");
  conn_.prepare("get_friend_leaderboard",
  "SELECT users.id, users.username, COALESCE("
  "(SELECT o.end_elo FROM matches m "
  "JOIN outcomes o ON m.id = o.match "
  "JOIN users u ON o.player = u.id "
  "WHERE u.id = users.id AND m.game = matches.game AND m.ranked = true "
  "ORDER BY m.id DESC "
  "LIMIT 1), 1000) as elo, "
  "(SELECT COUNT(outcome) "
  "FROM matches m "
  "JOIN outcomes o ON m.id = o.match "
  "WHERE o.player = users.id "
  "AND outcome = 'WIN' AND m.ranked = true "
  "AND m.game = matches.game) "
  "as wins, "
  "(SELECT COUNT(outcome) "
  "FROM matches m "
  "JOIN outcomes o ON m.id = o.match "
  "WHERE o.player = users.id "
  "AND outcome = 'LOSS' AND m.ranked = true "
  "AND m.game = matches.game) "
  "as losses, "
  "(SELECT COUNT(outcome) "
  "FROM matches m "
  "JOIN outcomes o ON m.id = o.match "
  "WHERE o.player = users.id "
  "AND outcome = 'DRAW' AND m.ranked = true "
  "AND m.game = matches.game) "
  "as draws "
  "FROM users "
  "JOIN outcomes ON outcomes.player = users.id "
  "JOIN matches ON outcomes.match = matches.id "
  "WHERE game = $1 "
  "AND users.id NOT IN (6, 7, 8) "
  "AND (users.id = $2 "
  "OR (users.id IN "
  "(SELECT friend1 FROM friends "
  "WHERE friend2 = $2)) "
  "OR (users.id IN "
  "(SELECT friend2 FROM friends "
  "WHERE friend1 = $2))) "
  "GROUP BY game, users.id "
  "ORDER BY elo DESC, username;");
  conn_.prepare("are_friends",
  "SELECT * FROM friends "
  "WHERE (friend1 = $1 AND friend2 = $2) "
  "OR (friend1 = $2 AND friend1 = $1);");
  conn_.prepare("get_friends_uid",
  "SELECT * FROM friends "
  "WHERE friend1 = $1 OR friend2 = $1;");
  conn_.prepare("get_incoming_freqs",
  "SELECT from_user FROM friendreqs "
  "WHERE to_user = $1;");
  conn_.prepare("get_outgoing_freqs",
  "SELECT to_user FROM friendreqs "
  "WHERE from_user = $1;");
  conn_.prepare("add_friend",
  "INSERT INTO friends "
  "VALUES ($1, $2);");
  conn_.prepare("delete_friend",
  "DELETE FROM friends "
  "WHERE friend1 = $1 AND friend2 = $2;");
  conn_.prepare("send_friend_req",
  "INSERT INTO friendreqs "
  "VALUES ($1, $2);");
  conn_.prepare("delete_friend_req",
  "DELETE FROM friendreqs "
  "WHERE from_user = $1 AND to_user = $2;");
  conn_.prepare("insert_var_code",
  "INSERT INTO varification_codes VALUES ($1, $2);");
  conn_.prepare("change_password",
  "UPDATE users SET password_hash = $1 WHERE id = $2;");
}

template<typename... Args>
auto DatabaseManager::execute(std::string statement, Args... args) -> pqxx::result {
  try {
    pqxx::work w(conn_);
    auto result = w.exec_prepared(statement, args...);
    w.commit();
    return result;
  } catch (const pqxx::pqxx_exception &e) {
    std::cerr << e.base().what();
  }
  return pqxx::result();
}

template<typename... Args>
auto DatabaseManager::execute0(std::string statement, Args... args) -> bool {
  try {
    pqxx::work w(conn_);
    auto res = w.exec_prepared0(statement, args...);
    w.commit();
    auto rows_affected = res.affected_rows();
    if (rows_affected > 0){
      return true;
    }
  } catch (const pqxx::pqxx_exception &e) {
    std::cerr << e.base().what();
    return false;
  }
  return false;
}

template<typename... Args>
auto DatabaseManager::execute1(std::string statement, Args... args) -> pqxx::row {
  try {
    pqxx::work w(conn_);
    auto row = w.exec_prepared1(statement, args...);
    return row;
  } catch (const pqxx::pqxx_exception &e) {
    std::cerr << e.base().what();
  }
  return pqxx::row();
}

template<typename... Args>
auto DatabaseManager::batch_insert(std::string table, std::vector<std::string> columns,
    std::vector<Args...> entries) -> bool {
  try {
    pqxx::work w(conn_);
    pqxx::stream_to stream {w, table, columns};
    for (const auto &entry : entries) {
      stream << entry;
    }
    stream.complete();
    w.commit();
  } catch (const pqxx::pqxx_exception &e) {
    std::cerr << e.base().what();
    return false;
  }
  return true;
}

auto DatabaseManager::parse_matches(pqxx::result res) -> std::vector<Match*> {
  auto matches = std::vector<Match*>{};
  auto curmatchID = -1;
  Match *curmatch = NULL;
  for (auto const &row : res) {
    auto match = atoi(row[0].c_str());
    // If different match, create a new match object.
    if (match != curmatchID) {
      if (curmatch != NULL) {
        matches.push_back(curmatch);
      }
      auto game = row[1].c_str();
      auto ranked = std::string(row[2].c_str()) == "t";
      auto potholes = row[3].c_str();
      auto replay = row[4].c_str();
      auto svg_data = row[5].c_str();
      curmatch = new Match(match, game, ranked, potholes, replay, svg_data, std::vector<Player>{});
      curmatchID = match;
    }
    // Add player
    auto p_id = atoi(row[6].c_str());
    auto p_username = row[7].c_str();
    auto p_startelo = atoi(row[8].c_str());
    auto p_endelo = atoi(row[9].c_str());
    auto p_outcome = row[10].c_str();
    auto *player = new Player(p_id, p_username, p_startelo, p_endelo, p_outcome);
    curmatch->players.push_back(*player);
  }
  // Push last.
  if (curmatch != NULL) {
    matches.push_back(curmatch);
  }
  return matches;
}
