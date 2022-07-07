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

    auto insert_user(std::string username, std::string email, std::string password) -> bool;
    auto get_user(std::string email) -> User*;
    auto get_user(int id) -> User*;

  private:
    auto prepare_statements() -> void;
    template<typename... Args>
    auto execute(std::string statement, Args... args) -> pqxx::result;
    template<typename... Args>
    auto execute0(std::string statement, Args... args) -> bool;
    template<typename... Args>
    auto execute1(std::string statement, Args... args) -> pqxx::row;
};

#endif