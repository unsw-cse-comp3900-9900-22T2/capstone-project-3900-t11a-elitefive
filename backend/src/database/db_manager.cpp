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

auto DatabaseManager::does_user_exist(std::string username) -> int {
  auto res = execute("username_get_user", username);
  if (res.size() != 1) {
    return -1;
  }
  return atoi(res[0][0].c_str());
}

auto DatabaseManager::save_match(std::string gameType, std::map<int, int> playersELO, int winner, std::string move_seq,
    std::vector<uint64_t> snapshots) -> int {
  // Insert Match
  auto res = execute("insert_match", gameType, move_seq);
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

auto DatabaseManager::get_matches() -> std::vector<Match> {
  auto res = execute("get_matches");
  return parse_matches(res);
}

auto DatabaseManager::get_matches(int id) -> std::vector<Match> {
  auto res = execute("get_user_matches", id);
  return parse_matches(res);
}

auto DatabaseManager::get_latest_elo(int id, std::string gameType) -> int {
  auto res = execute("get_latest_elo", id, gameType);
  if (res.size() != 1) {
    return 0;
  }
  return atoi(res[0][0].c_str());
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
  "INSERT INTO matches(game, replay) "
  "VALUES ($1, $2) "
  "RETURNING id;");
  // TODO: Refactor the two below - similar SQL.
  conn_.prepare("get_matches",
  "SELECT matches.id, game, replay, outcomes.player, "
  "username, end_elo, outcome "
  "FROM matches "
  "JOIN outcomes ON matches.id = outcomes.match "
  "JOIN users ON outcomes.player = users.id;");
  conn_.prepare("get_user_matches",
  "SELECT matches.id, game, replay, outcomes.player, "
  "username, end_elo, outcome "
  "FROM matches "
  "JOIN outcomes ON matches.id = outcomes.match "
  "JOIN users ON outcomes.player = users.id "
  "WHERE match IN "
  "(SELECT matches.id FROM matches "
  "JOIN outcomes ON matches.id = outcomes.match "
  "WHERE outcomes.player = $1);");
  conn_.prepare("get_latest_elo",
  "SELECT end_elo "
  "FROM outcomes "
  "JOIN matches ON outcomes.match = matches.id "
  "WHERE player = $1 "
  "AND game = $2 "
  "ORDER BY end_time DESC "
  "LIMIT 1");
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
    w.exec_prepared0(statement, args...);
    w.commit();
    return true;
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
