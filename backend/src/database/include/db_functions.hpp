#ifndef DATABASE_FUNCTIONS_HPP
#define DATABASE_FUNCTIONS_HPP

#include <string>
#include <iostream>
#include "db_classes.hpp"

// USER

// Get user when given id 
auto db_get_user(int id) -> User*;

// Get user with given email
auto db_get_user(std::string email) -> User*;

// Insert user into database
void db_insert_user(std::string username, std::string email, std::string password_hash);

// FRIENDS

// Add friends using their IDs
auto db_add_friends(int friend1, int friend2) -> int;

#endif