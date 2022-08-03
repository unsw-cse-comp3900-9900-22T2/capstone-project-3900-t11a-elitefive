#include <string>
#include <iostream>

#include <pqxx/pqxx>

#include "db_classes.hpp"

auto hash_password(std::string password) -> std::string;

auto check_password(std::string email, std::string password) -> bool;