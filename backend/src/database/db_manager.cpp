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
  if (res.size() != 1) {
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

auto DatabaseManager::save_match(std::string gameType, bool is_ranked, std::map<int, int> playersELO, int winner, std::string move_seq,
    std::vector<uint64_t> snapshots) -> int {
  // Insert Match
  auto res = execute("insert_match", gameType, is_ranked, move_seq);
  auto matchID = atoi(res[0][0].c_str());
  // Insert Players & Outcomes
  auto outcome_columns = std::vector<std::string>{"player", "match", "end_elo", "outcome"};
  auto outcome_entries = std::vector<std::tuple<int, int, int, std::string>>{};
  for (auto const& entry : playersELO) {
      auto player = entry.first;
      auto end_elo = entry.second;
      auto outcome = (playersELO.count(winner) == 0) ? "DRAW" : (winner == player) ? "WIN" : "LOSS";
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
  auto replay = res[0][2].c_str();
  auto curmatch = new Match(match, game, replay, std::vector<Player>{});
  for (auto const &row : res) {
    // Add player
    auto p_id = atoi(row[3].c_str());
    auto p_username = row[4].c_str();
    auto p_endelo = atoi(row[5].c_str());
    auto p_outcome = row[6].c_str();
    auto *player = new Player(p_id, p_username, p_endelo, p_outcome);
    curmatch->players.push_back(*player);
  }
  return curmatch;
}

auto DatabaseManager::get_matches() -> std::vector<Match> {
  auto res = execute("get_matches");
  return parse_matches(res);
}

auto DatabaseManager::get_matches(int id) -> std::vector<Match> {
  auto res = execute("get_matches_user", id);
  return parse_matches(res);
}

auto DatabaseManager::get_matches(int move_n, int64_t bs) -> std::vector<Match> {
  auto res = execute("get_matches_snapshot1", move_n, bs);
  return parse_matches(res);
}

auto DatabaseManager::get_matches(int move_n1, int64_t bs1, int move_n2, int64_t bs2) -> std::vector<Match> {
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
    auto ranked = row[1].c_str() == "t";
    auto outcome = row[2].c_str();
    auto key = std::string(game).append(ranked ? " RANKED " : " UNRANKED ").append(outcome);
    auto count = atoi(row[3].c_str());
    stats.insert({key, count});
  }
  auto result = new PlayerStats(stats);
  return result;
}

auto DatabaseManager::get_elo_progress(int id) -> std::map<std::string, std::vector<int>> {
  auto res = execute("get_elo_progress", id);
  auto progress = std::map<std::string, std::vector<int>>{
    {"CLASSIC", {}},
    {"TRIPLES", {}},
    {"POTHOLES", {}}
  };
  for (auto const &row : res) {
    auto game = row[0].c_str();
    auto elo = atoi(row[1].c_str());
    progress[game].push_back(elo);
  }
  return progress;
}

auto DatabaseManager::get_friends(int id) -> std::vector<User*> {
  auto friends = std::vector<User*>();
  auto res = execute("get_friends_uid", id);
  for (auto row : res) {
    std::cout << row[0] << " "  << row[1] << "\n";
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
  "INSERT INTO matches(game, ranked, replay) "
  "VALUES ($1, $2, $3) "
  "RETURNING id;");
  conn_.prepare("get_match",
  "SELECT matches.id, game, replay, outcomes.player, "
  "username, end_elo, outcome "
  "FROM matches "
  "JOIN outcomes ON matches.id = outcomes.match "
  "JOIN users ON outcomes.player = users.id "
  "WHERE matches.id = $1;");
  // TODO: Refactor the two below - similar SQL.
  conn_.prepare("get_matches",
  "SELECT matches.id, game, replay, outcomes.player, "
  "username, end_elo, outcome "
  "FROM matches "
  "JOIN outcomes ON matches.id = outcomes.match "
  "JOIN users ON outcomes.player = users.id;");
  conn_.prepare("get_matches_user",
  "SELECT matches.id, game, replay, outcomes.player, "
  "username, end_elo, outcome "
  "FROM matches "
  "JOIN outcomes ON matches.id = outcomes.match "
  "JOIN users ON outcomes.player = users.id "
  "WHERE match IN "
  "(SELECT matches.id FROM matches "
  "JOIN outcomes ON matches.id = outcomes.match "
  "WHERE outcomes.player = $1);");
  conn_.prepare("get_matches_snapshot1",
  "SELECT matches.id, game, replay, outcomes.player, "
  "username, end_elo, outcome "
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
  "SELECT matches.id, game, replay, outcomes.player, "
  "username, end_elo, outcome "
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
  "ORDER BY end_time DESC "
  "LIMIT 1), 1000);");
  conn_.prepare("get_stats",
  "SELECT game, ranked, outcome, count(outcome) "
  "FROM outcomes "
  "JOIN matches ON outcomes.match = matches.id "
  "WHERE player = $1 "
  "GROUP BY game, ranked, outcome;");
  conn_.prepare("get_elo_progress",
  "SELECT game, end_elo "
  "FROM outcomes "
  "JOIN matches ON outcomes.match = matches.id "
  "WHERE player = $1 AND ranked = true "
  "GROUP BY game, end_elo, end_time "
  "ORDER BY game, end_time");
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
    auto rows_deleted = res.affected_rows();
    if (rows_deleted > 0){
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

auto DatabaseManager::parse_matches(pqxx::result res) -> std::vector<Match> {
  auto matches = std::vector<Match>{};
  auto curmatchID = -1;
  Match *curmatch = NULL;
  for (auto const &row : res) {
    auto match = atoi(row[0].c_str());
    // If different match, create a new match object.
    if (match != curmatchID) {
      if (curmatch != NULL) {
        matches.push_back(*curmatch);
      }
      auto game = row[1].c_str();
      auto replay = row[2].c_str();
      curmatch = new Match(match, game, replay, std::vector<Player>{});
      curmatchID = match;
    }
    // Add player
    auto p_id = atoi(row[3].c_str());
    auto p_username = row[4].c_str();
    auto p_endelo = atoi(row[5].c_str());
    auto p_outcome = row[6].c_str();
    auto *player = new Player(p_id, p_username, p_endelo, p_outcome);
    curmatch->players.push_back(*player);
  }
  // Push last.
  if (curmatch != NULL) {
    matches.push_back(*curmatch);
  }
  return matches;
}
