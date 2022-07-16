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
  	if (res.size() != 1){
			return NULL;
		}
  return new User(res[0]);
}

auto DatabaseManager::get_user(int id) -> User* {  
    auto res = execute("id_get_user", id);
  	if (res.size() != 1){
			return NULL;
		}
  return new User(res[0]);
}

auto DatabaseManager::save_match(std::string gameType, std::string move_seq) -> int {
  auto row = execute1("insert_match", gameType, move_seq);
  return atoi(row[0].c_str());
}

auto DatabaseManager::get_friends(int id) -> std::vector<User*> {
  auto friends = std::vector<User*>();
    auto res = execute("get_friends_uid", id);
    for (auto row : res){
      std::cout << row[0] << " "  << row[1] << "\n";
      if (atoi(row[0].c_str()) != id){
            friends.push_back(get_user(atoi(row[0].c_str())));
      }
      if (atoi(row[1].c_str()) != id){
            friends.push_back(get_user(atoi(row[1].c_str())));
      }
    }
  return friends;
}

auto DatabaseManager::add_friend(int id1, int id2) -> bool {
 
  auto fri1 = std::min(id1,id2);
  auto fri2 = std::max(id1,id2);
  
  return execute0("add_friend", fri1, fri2);
}

auto DatabaseManager::delete_friend(int id1, int id2) -> bool {
  auto fri1 = std::min(id1,id2);
  auto fri2 = std::max(id1,id2);
  
  pqxx::work w(conn_);
  try {
    auto res = w.exec_prepared0("delete_friend", fri1, fri2);
    w.commit();
    auto rows_deleted = res.affected_rows();
    if (rows_deleted > 0){
      return true;
    }
  } catch (const pqxx::pqxx_exception &e) {
    return false;
  }
  return false;
}

// Prepare the statements on instantiation.

auto DatabaseManager::prepare_statements() -> void {
  conn_.prepare("insert_user", "INSERT INTO users VALUES (DEFAULT, $1, $2, $3);");
  conn_.prepare("email_get_user", "SELECT * FROM users WHERE email = $1;");
  conn_.prepare("id_get_user", "SELECT * FROM users WHERE id = $1;");
  conn_.prepare("insert_match", "INSERT INTO matches(game, replay) VALUES ($1, $2) RETURNING id;");
  conn_.prepare("get_friends_uid", "SELECT * FROM friends WHERE friend1 = $1 or friend2 = $1;");
  conn_.prepare("add_friend", "INSERT INTO friends VALUES ($1, $2);");
  conn_.prepare("delete_friend","delete from friends where friend1 = $1 and friend2 = $2;");
}

template<typename... Args>
auto DatabaseManager::execute(std::string statement, Args... args) -> pqxx::result {
  pqxx::work w(conn_);
  try {
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
  pqxx::work w(conn_);
  try {
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

  pqxx::work w(conn_);
  try {
    auto row = w.exec_prepared1(statement, args...);
    return row;
  } catch (const pqxx::pqxx_exception &e) {
    std::cerr << e.base().what();
  }
  return pqxx::row();
}
