#include <string>
#include "db_manager.hpp"
#include "db_utils.hpp"
#include "server_util.hpp"
#include <nlohmann/json.hpp>
#include <random>
#include <filesystem>

using json = nlohmann::ordered_json;

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

auto generate_temporary_password() -> std::string {

    srand(time(NULL) + rand());
    auto token = std::string();
    for(int i = 0;i < 16;i++)
    {
        int r = 'A' + (rand() % 25);
        while(r < 16)
            // caps letter
            r = 'A' + (rand() % 25) ;
            // integer
            if (r % 3 == 1){
                r = '0' + (rand() % 10) ;
            }
            // lowercase letter
            if (r % 3 == 2){
                r = 'a' + (rand() % 25) ;
            }
        token.push_back(char(r));
    }
	return token;
}

auto random_num(int bound) -> int {
    if (bound <= 0) {
        return -1;
    }
    auto seed = std::chrono::steady_clock::now().time_since_epoch().count();
    auto e = std::default_random_engine(seed);
    auto distribution = std::uniform_int_distribution<int>(0, bound - 1);
    auto random = distribution(e);
    return random;
}

auto send_email_welcome(std::string email, std::string username) -> void{
	
	auto message = std::string("Hello ") + username + std::string("!! \\n")
	+ std::string("Account creation successful.\\nWelcome to Yavalath!!");

    auto wd = std::filesystem::current_path();
	auto cmd = wd + "/mail/send_email.sh " + email + " \"Welcome to Yavalath!\" \"" + message + " \"";
		
	std::system(cmd.c_str());
}

auto send_email_temp_password(std::string email, std::string username, std::string temp_pass) -> void {
	
	auto message = std::string("Hi ") + username + std::string("!! \\n")
	+ std::string("Your temporary Yavalath password is: ") + temp_pass;

	auto cmd = "/app/mail/send_email.sh " + email + " \"Yavalath: Forgotten Password\" \"" + message + " \"";
		
	std::system(cmd.c_str());
}

auto profile_to_json(User *user, PlayerStats *stats, std::map<std::string, int> elos,
    std::map<std::string, std::vector<int>> elohistory, std::vector<Match*> matchhistory, std::vector<User*> friends) -> json {
    nlohmann::ordered_json result;
    result["event"] = "profile";
    result["action"] = "get";
    result["payload"]["uid"] = user->id;
    result["payload"]["username"] = user->username;
    result["payload"]["email"] = user->email;
    result["payload"]["ranked"] = stats_to_json(true, stats, elos);
    result["payload"]["unranked"] = stats_to_json(false, stats, elos);
    result["payload"]["elo_history"] = elo_history_to_json(elohistory);
    result["payload"]["match_history"] = match_history_to_json(matchhistory);
    result["payload"]["match_history_filtered"] = match_history_filtered_to_json(matchhistory);
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

auto elo_history_to_json(std::map<std::string, std::vector<int>> elohistory) -> json {
    json result;
    std::vector<std::string> modes = { "CLASSIC", "TRIPLES", "POTHOLES" };
    for (auto const& mode : modes) {
        result[mode] = elohistory.at(mode);
    }
    return result;
}

auto match_history_filtered_to_json(std::vector<Match*> matchhistory) -> json { 
    // Sort by showing most recent matches first
    std::stable_sort(matchhistory.begin(), matchhistory.end(), [](Match *const& a, Match *const& b) {
        return a->id > b->id;
    });
    
    // FILTERED
    json result;
    result["CLASSIC"] = {};
    result["TRIPLES"] = {};
    result["POTHOLES"] = {};

    for (auto const& match : matchhistory) {
        json m = match->to_json();
        std::string mode = m["gamemode"];
        result[mode].push_back(m);
    }
    return result;
}

auto match_history_to_json(std::vector<Match*> matchhistory) -> json {
    // TODO: DELETE BASIC
    // Sort by showing most recent matches first
    std::stable_sort(matchhistory.begin(), matchhistory.end(), [](Match *const& a, Match *const& b) {
        return a->id > b->id;
    });
    nlohmann::ordered_json result = {};
    for (auto const& match : matchhistory) {
        result.push_back(match->to_json());
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

auto social_json(std::string link, std::string message) -> json {
    json result = {};
    if (link.empty()) {
        result["has-link"] = false;
    } else {
        result["has-link"] = true;
        result["link"] = link;
    }
    result["message"] = message;
    return result;
}