#ifndef SERVER_UTILS_HPP
#define SERVER_UTILS_HPP

#include <string>

using json = nlohmann::json;

auto generate_session_token(int id) -> std::string;

auto generate_varification_code() -> std::string;

auto send_email_varification(std::string email, std::string username, std::string var_code) -> void;

auto profile_to_json(User *user, PlayerStats *stats, std::map<std::string, int> elos, std::vector<User*> friends) -> json;

auto stats_to_json(bool ranked, PlayerStats *stats, std::map<std::string, int> elos) -> json;

auto all_friends_to_json(int id, std::vector<User*> friends, std::vector<User*> incoming, std::vector<User*> outgoing) -> json;

auto friends_to_json(std::vector<User*> friends) -> json;

#endif