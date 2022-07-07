#include <string>
#include <iostream>

#include <pqxx/pqxx>

auto hash_password(std::string password) -> std::string;