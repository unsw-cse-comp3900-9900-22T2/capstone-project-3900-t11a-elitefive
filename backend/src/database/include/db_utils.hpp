#include <string>
#include <iostream>

#include <pqxx/pqxx>

#include "db_classes.hpp"

auto hash_password(std::string password) -> std::string;
auto friends_to_json(int id, std::vector<User*> friends) -> std::string;