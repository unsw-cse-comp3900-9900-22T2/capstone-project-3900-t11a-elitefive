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

class Player {
  public:
    int id;
    std::string username;
    int start_elo;
    int end_elo;
    std::string outcome;

    Player(int id_, std::string username_, int start_elo_, int end_elo_, std::string outcome_)
    : id{id_}
    , username{username_}
    , start_elo{start_elo_}
    , end_elo{end_elo_}
    , outcome{outcome_} {}

    auto to_json() const -> json {
      json payload;
      payload["uid"] = id;
      payload["username"] = username;
      payload["elo_start"] = start_elo;
      payload["elo_end"] = end_elo;
      payload["outcome"] = outcome;
      return payload;
    }
};

class Match {
  public:
    int id;
    std::string game;
    bool ranked;
    std::string potholes;
    std::string replay;
    std::vector<Player> players;

    Match(int id_, std::string game_, bool ranked_, std::string potholes_,
     std::string replay_, std::vector<Player> players_)
    : id{id_}
    , game{game_}
    , ranked{ranked_}
    , potholes{potholes_}
    , replay{replay_}
    , players{players_} {}

    auto to_json() const -> json {
      json payload;
      payload["match_id"] = id;
      payload["gamemode"] = game;
      payload["ranked"] = ranked;
      payload["potholes"] = potholes;
      payload["moves"] = replay;
      payload["players"] = {};
      payload["link"] = "/replay/" + std::to_string(id);
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

class LeaderboardEntry {
  public:
    int rank;
    int uid;
    std::string username;
    int elo;
    int wins;
    int losses;
    int draws;

    LeaderboardEntry(int rank_, int uid_, std::string username_,
      int elo_, int wins_, int losses_, int draws_)
    : rank{rank_}
    , uid{uid_}
    , username{username_}
    , elo{elo_}
    , wins{wins_}
    , losses{losses_}
    , draws{draws_} {}

    auto to_json() const -> json {
      json payload;
      payload["rank"] = rank;
      payload["uid"] = uid;
      payload["username"] = username;
      payload["elo"] = elo;
      payload["wins"] = wins;
      payload["losses"] = losses;
      payload["draws"] = draws;
      return payload;
    }
};

#endif