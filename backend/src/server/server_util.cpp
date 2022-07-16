#include <string>
#include "db_manager.hpp"
#include "db_utils.hpp"
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

auto profile_to_json(User *user, PlayerStats *stats, std::vector<User*> friends) -> std::string {

    auto profile_json = json{
        {"event", "profile"},
        {"action",  "get"},
        {"payload", {
            {"uid",  user->id},
            {"username",  user->username},
            {"email", user->email},
            {"classic", {{"elo", 0},  {"wins", 0},  {"losses", 0}, {"draws",0},}},
            {"triples", {{"elo", 0},  {"wins", 0},  {"losses", 0}, {"draws",0},}},
            {"potholes", {{"elo", 0},  {"wins", 0},  {"losses", 0}, {"draws",0},}},
            {"friends", {}}
            }
        },
    };
        
    return profile_json.dump();
}

