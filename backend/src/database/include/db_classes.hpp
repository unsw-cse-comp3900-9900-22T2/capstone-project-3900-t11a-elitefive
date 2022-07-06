#ifndef DATABASE_CLASSES_HPP
#define DATABASE_CLASSES_HPP

#include <unordered_set>

// Classes for database tuples

// User Table:
// id serial,
// username text,
// email text,
// password_hash text,
// primary key (id)
class User {       
  public:            
    int id;        
    std::string username;
    std::string email;
    std::string password_hash;
    // std::unordered_set<int> friends;
};

#endif