#include <string>
#include <iostream>

#include <pqxx/pqxx>

auto execute_query(std::string sql) -> pqxx::result;

auto commit_query(std::string sql) -> int;

auto hash_password(std::string password) -> std::string;