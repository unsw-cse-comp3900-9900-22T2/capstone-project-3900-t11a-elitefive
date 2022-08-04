#ifndef DATABASE_MANAGER_HPP
#define DATABASE_MANAGER_HPP

#include <iostream>
#include "db_classes.hpp"
#include <pqxx/pqxx>

auto const DB_STRING = std::string("user=lubuntu dbname=yavalath host=localhost port=5432");

class DatabaseManager {
  private:
    pqxx::connection conn_;

  public:
    DatabaseManager();

    // USER
    auto insert_user(std::string username, std::string email, std::string password) -> bool;
    auto get_user(std::string email) -> User*;
    auto get_user(int id) -> User*;
    auto get_user_username(std::string username) -> User*;
    // MATCHES
    auto save_match(std::string gameType, bool is_ranked, std::map<int, int> playersELO, int winner,
      std::string potholes, std::string move_seq, std::string svg_data, std::vector<uint64_t> snapshots) -> int;
    auto save_match(std::string gameType, bool is_ranked, std::map<int, int> playersELO, std::map<int, std::string> playerOutcomes,
      std::string potholes, std::string move_seq, std::string svg_data, std::vector<uint64_t> snapshots) -> int;
    auto get_match(int id) -> Match*;
    auto get_matches() -> std::vector<Match*>;
    auto get_matches(int id) -> std::vector<Match*>;
    auto get_last_match(int id) -> Match*;
    // SNAPSHOTS
    auto get_matches(int move_n, int64_t bs) -> std::vector<Match*>;
    auto get_matches(int move_n1, int64_t bs1, int move_n2, int64_t bs2) -> std::vector<Match*>;
    // PLAYER PROFILE
    auto get_latest_elo(int id, std::string gameType) -> int;
    auto get_stats(int id) -> PlayerStats*;
    auto get_elo_progress(int id, std::string mode) -> std::vector<int>;
    // LEADERBOARD
    auto get_global_leaderboard(std::string gameType) -> std::vector<LeaderboardEntry>;
    auto get_friend_leaderboard(std::string gameType, int id) -> std::vector<LeaderboardEntry>;
    auto get_global_rank(std::string gameType, int id) -> int;
    // FRIENDS
    auto get_friends(int id) -> std::vector<User*>;
    auto are_friends(int id1, int id2) -> bool;
    auto get_incoming_freqs(int id) -> std::vector<User*>;
    auto get_outgoing_freqs(int id) -> std::vector<User*>;
    auto delete_friend(int from, int to) -> bool;
    auto send_friend_req(int from, int to) -> bool;
    auto accept_friend_req(int accepter, int accepted) -> bool;
    auto deny_friend_req(int denier, int denied) -> bool;
    auto revoke_friend_req(int revoker, int revoked) -> bool;
    // MAIL
    auto insert_varification_code(int user, std::string var_code) -> bool;
    auto change_password(int user, std::string new_password) -> bool;
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
    auto parse_matches(pqxx::result res) -> std::vector<Match*>;
};

#endif