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

class Player {
  public:
    int id;
    std::string username;
    int end_elo;
    std::string outcome;

    Player(int id_, std::string username_, int end_elo_, std::string outcome_)
    : id{id_}
    , username{username_}
    , end_elo{end_elo_}
    , outcome{outcome_} {}
};

class Match {
  public:
    int id;
    std::string game;
    std::string replay;
    std::vector<Player> players;

    Match(int id_, std::string game_, std::string replay_, std::vector<Player> players_)
    : id{id_}
    , game{game_}
    , replay{replay_}
    , players{players_} {}
};

class PlayerStats {
  public:
    int wins;
    int losses;
    int draws;
  
    PlayerStats(int wins_, int losses_, int draws_)
    : wins{wins_}
    , losses{losses_}
    , draws(draws_) {}
}

#endif