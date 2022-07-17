#ifndef DATABASE_CLASSES_HPP
#define DATABASE_CLASSES_HPP

#include <unordered_set>
#include <pqxx/pqxx>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

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

/*
class ProfileStats {
  public: 
    int classic_elo;
    int classic_wins;
    int classic_loses;
    int classic_draws;
    
    ProfileStats()
    : classic_elo{0}
    , classic_wins{0}
    , classic_loses{0}
    , classic_draws{0} {}
};
*/


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

    auto to_json() const -> json {
        json payload;
        payload["uid"] = id;
        payload["username"] = username;
        payload["elo_end"] = end_elo;
        payload["elo_start"] = 0;
        payload["outcome"] = outcome;
        return payload;
    }
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

    auto to_json() const -> json {
        json payload;
        payload["match_id"] = id;
        payload["gamemode"] = game;
        payload["moves"] = replay;
        payload["players"] = {};
        for (auto const& player : players) {
            payload["players"].push_back(player.to_json());
        }
        return payload;
    }
};

class PlayerStats {
  private:
    std::map<std::string, int> stats;
  
    auto get_outcome(std::string game, bool ranked, std::string outcome) -> int {
      return stats[game.append(ranked ? " RANKED " : " UNRANKED ").append(outcome)];
    }

  public:
    PlayerStats(std::map<std::string, int> stats_) 
    : stats{stats_} {}

    auto get_WLD(std::string game, bool ranked) -> std::vector<int> {
      return
      { get_outcome(game, ranked, "WIN"),
        get_outcome(game, ranked, "LOSS"),
        get_outcome(game, ranked, "DRAW") };
    }

    auto get_wins(std::string game, bool ranked) -> int {
      return get_WLD(game, ranked)[0];
    }

    auto get_losses(std::string game, bool ranked) -> int {
      return get_WLD(game, ranked)[1];
    }

    auto get_draws(std::string game, bool ranked) -> int {
      return get_WLD(game, ranked)[2];
    }
};

#endif