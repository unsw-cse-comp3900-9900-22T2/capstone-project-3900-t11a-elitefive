#include <unordered_set>

// Classes for database tuples

class User {       
  public:            
    int id;        
    std::string username;
    std::string email;
    std::string password_hash;
    std::unordered_set<int> friends;
};
