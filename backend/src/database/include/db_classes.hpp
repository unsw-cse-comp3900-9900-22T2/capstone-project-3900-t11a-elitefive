#ifndef DATABASE_CLASSES_HPP
#define DATABASE_CLASSES_HPP

#include <unordered_set>
#include <pqxx/pqxx>

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

    User(pqxx::row const& row)
    : id{atoi(row[0].c_str())}
    , username{row[1].c_str()}
    , email{row[2].c_str()}
    , password_hash{row[3].c_str()} {}

};

#endif