#include "db_manager.hpp"

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
  return execute0("insert_user", username, email, password);
}

auto DatabaseManager::get_user(std::string email) -> User* {
  auto row = execute1("email_get_user", email);
	return new User(row);
}

auto DatabaseManager::get_user(int id) -> User* {
  auto row = execute1("id_get_user", id);
  return new User(row);
}

auto DatabaseManager::save_match(std::string gameType, std::map<int, int> playersELO, int winner, std::string move_seq,
    std::vector<uint64_t> snapshots) -> int {
  // Insert Match
  auto row = execute1("insert_match", gameType, move_seq);
  auto matchID = atoi(row[0].c_str());
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

// Prepare the statements on instantiation.
auto DatabaseManager::prepare_statements() -> void {
  conn_.prepare("insert_user", "INSERT INTO users VALUES (DEFAULT, $1, $2, $3);");
  conn_.prepare("email_get_user", "SELECT * FROM users WHERE email = $1;");
  conn_.prepare("id_get_user", "SELECT * FROM users WHERE id = $1;");
  conn_.prepare("insert_match", "INSERT INTO matches(game, replay) VALUES ($1, $2) RETURNING id;");
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
    w.commit();
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