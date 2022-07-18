#ifndef SERVER_UTILS_HPP
#define SERVER_UTILS_HPP

#include <string>

using json = nlohmann::json;

auto generate_session_token(int id) -> std::string;

auto profile_to_json(User *user, PlayerStats *stats, std::vector<User*> friends) -> json;

auto all_friends_to_json(int id, std::vector<User*> friends, std::vector<User*> incoming, std::vector<User*> outgoing) -> json;

auto friends_to_json(std::vector<User*> friends) -> json;

#endif