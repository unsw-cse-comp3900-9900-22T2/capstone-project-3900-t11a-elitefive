#include <string>
#include "db_manager.hpp"
#include "db_utils.hpp"
#include "server_util.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

auto generate_session_token(int id) -> std::string {

    srand(time(NULL) + id + rand());
    auto token = std::string();
    for(int i = 0;i < 64;i++)
    {
        int r = rand() % 127;
        while(r < 32)
            r = rand() % 127;
            if (r == '\n' || r == '"' || r == '\''){
                r = 'x';
            }
        token.push_back(char(r));
    }
	return token;
}

auto profile_to_json(User *user, PlayerStats *stats, std::map<std::string, int> elos, std::vector<User*> friends) -> json {
    json result;
    result["event"] = "profile";
    result["action"] = "get";
    result["payload"]["uid"] = user->id;
    result["payload"]["username"] = user->username;
    result["payload"]["email"] = user->email;
    result["payload"]["ranked"] = stats_to_json(true, stats, elos);
    result["payload"]["unranked"] = stats_to_json(false, stats, elos);
    result["payload"]["friends"] = friends_to_json(friends);
    return result;
}

auto stats_to_json(bool ranked, PlayerStats *stats, std::map<std::string, int> elos) -> json {
    json result;
    std::vector<std::string> modes = { "CLASSIC", "TRIPLES", "POTHOLES" };
    for (auto const& mode : modes) {
        result[mode]["wins"] = stats->get_wins(mode, ranked);
        result[mode]["losses"] = stats->get_losses(mode, ranked);
        result[mode]["draws"] = stats->get_draws(mode, ranked);
        if (ranked) {
            result[mode]["elo"] = elos.at(mode);
        }
    }
    return result;
}

auto all_friends_to_json(int id, std::vector<User*> friends, std::vector<User*> incoming, std::vector<User*> outgoing) -> json {
    json result;
    result["event"] = "friends";
    result["action"] = "get";
    result["payload"]["user"] = std::to_string(id);
    result["payload"]["friends"] = friends_to_json(friends);
    result["payload"]["incoming"] = friends_to_json(incoming);
    result["payload"]["outgoing"] = friends_to_json(outgoing);
    return result;
}

auto friends_to_json(std::vector<User*> friends) -> json {
    json result = {};
    for (auto const& fri : friends) {
        json r;
        r["id"] = fri->id;
        r["username"] = fri->username;
        result.push_back(r);
    }
    return result;
}