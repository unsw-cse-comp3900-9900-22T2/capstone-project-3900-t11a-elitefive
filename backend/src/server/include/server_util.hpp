#ifndef SERVER_UTILS_HPP
#define SERVER_UTILS_HPP

#include <string>

auto generate_session_token(int id) -> std::string;
auto profile_to_json(User *user, PlayerStats *stats, std::vector<User*> friends) -> std::string;

#endif