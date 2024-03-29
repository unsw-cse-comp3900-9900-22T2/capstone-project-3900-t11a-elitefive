#ifndef SERVER_UTILS_HPP
#define SERVER_UTILS_HPP

#include <string>

using json = nlohmann::ordered_json;

auto generate_session_token(int id) -> std::string;

auto generate_temporary_password() -> std::string;

auto random_num(int bound) -> int;

auto send_email_welcome(std::string email, std::string username) -> void;

auto send_email_temp_password(std::string email, std::string username,std::string temp_pass) -> void;

auto profile_to_json(User *user, PlayerStats *stats, std::map<std::string, int> elos,
  std::map<std::string, std::vector<int>> elohistory, std::vector<Match*> matchhistory, std::vector<User*> friends) -> json;

auto stats_to_json(bool ranked, PlayerStats *stats, std::map<std::string, int> elos) -> json;

auto elo_history_to_json(std::map<std::string, std::vector<int>> elohistory) -> json;

auto match_history_filtered_to_json(std::vector<Match*> matchhistory) -> json;

auto match_history_to_json(std::vector<Match*> matchhistory) -> json;

auto all_friends_to_json(int id, std::vector<User*> friends, std::vector<User*> incoming, std::vector<User*> outgoing) -> json;

auto friends_to_json(std::vector<User*> friends) -> json;

auto social_json(std::string link, std::string message) -> json;

#endif