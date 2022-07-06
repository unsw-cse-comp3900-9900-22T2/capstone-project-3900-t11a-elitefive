#include <string>
#include <iostream>
#include "db_classes.hpp"

// Insert user into database
// User Table:
// id serial,
// username text,
// email text,
// password_hash text,
// primary key (id)
void db_insert_user(std::string username, std::string email, std::string password_hash);

// Get user when given id 
auto db_get_user(int id) -> User*;

// Get user with given email
auto db_get_user(std::string email) -> User*;

// Add firends using their IDs
auto db_add_friends(int friend1, int friend2) -> int;

// Hash password with sha256
//auto hash_password(std::string password) -> std::string;