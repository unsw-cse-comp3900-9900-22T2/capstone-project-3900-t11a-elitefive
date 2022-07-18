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

auto profile_to_json(User *user, PlayerStats *stats, std::vector<User*> friends) -> json {
    json result;
    result["event"] = "profile";
    result["action"] = "get";
    result["payload"] = {};
    result["payload"]["uid"] = user->id;
    result["payload"]["username"] = user->username;
    result["payload"]["email"] = user->email;
    result["payload"]["classic"] = {};
    result["payload"]["triples"] = {};
    result["payload"]["potholes"] = {};
    result["payload"]["friends"] = friends_to_json(friends);
    // {"classic", {{"elo", 0},  {"wins", 0},  {"losses", 0}, {"draws",0},}},
    // {"triples", {{"elo", 100},  {"wins", 0},  {"losses", 0}, {"draws",0},}},
    // {"potholes", {{"elo", 0},  {"wins", 0},  {"losses", 0}, {"draws",0},}},
    return result;
}

auto all_friends_to_json(int id, std::vector<User*> friends, std::vector<User*> incoming, std::vector<User*> outgoing) -> json {
    json result;
    result["event"] = "friends";
    result["action"] = "get";
    result["payload"] = {};
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