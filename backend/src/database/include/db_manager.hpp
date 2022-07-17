#ifndef DATABASE_MANAGER_HPP
#define DATABASE_MANAGER_HPP

#include <iostream>
#include "db_classes.hpp"
#include <pqxx/pqxx>

auto const DB_STRING = std::string("user=yavalath dbname=yavalath password=yavalath host=postgres port=5432");

class DatabaseManager {
  private:
    pqxx::connection conn_;

  public:
    DatabaseManager();

    // USER
    auto insert_user(std::string username, std::string email, std::string password) -> bool;
    auto get_user(std::string email) -> User*;
    auto get_user(int id) -> User*;
    auto does_user_exist(std::string username) -> int;
    // MATCHES
    auto save_match(std::string gameType, bool is_ranked, std::map<int, int> playersELO, int winner, std::string move_seq,
      std::vector<uint64_t> snapshots) -> int;
    auto get_match(int id) -> Match*;
    auto get_matches() -> std::vector<Match>;
    auto get_matches(int id) -> std::vector<Match>;
    // SNAPSHOTS
    auto get_matches(int move_n, int64_t bs) -> std::vector<Match>;
    auto get_matches(int move_n1, int64_t bs1, int move_n2, int64_t bs2) -> std::vector<Match>;
    // PLAYER PROFILE
    auto get_latest_elo(int id, std::string gameType) -> int;
    auto get_stats(int id) -> PlayerStats*;
    auto get_elo_progress(int id) -> std::map<std::string, std::vector<int>>;
    // FRIENDS
    auto get_friends(int id) -> std::vector<User*>;
    auto delete_friend(int from, int to) -> bool;
    auto send_friend_req(int from, int to) -> bool;
    auto accept_friend_req(int from, int to) -> bool;
    auto deny_friend_req(int from, int to) -> bool;
  private:
    auto prepare_statements() -> void;
    // Execute query with return result.
    template<typename... Args>
    auto execute(std::string statement, Args... args) -> pqxx::result;
    // Execute query with no return result.
    template<typename... Args>
    auto execute0(std::string statement, Args... args) -> bool;
    // Execute query with only 1 return row.
    template<typename... Args>
    auto execute1(std::string statement, Args... args) -> pqxx::row;
    // Insert multiple rows.
    template<typename... Args>
    auto batch_insert(std::string table, std::vector<std::string> columns,
      std::vector<Args...> entries) -> bool;
    // Parse matches
    auto parse_matches(pqxx::result res) -> std::vector<Match>;
};

#endif