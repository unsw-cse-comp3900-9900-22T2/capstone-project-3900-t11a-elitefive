#include "api.hpp"
#include "App.h"
#include "db_manager.hpp"
#include "db_utils.hpp"
#include "server_util.hpp"
#include "metadatagen.hpp"


using json = nlohmann::json;

// POST REQUESTS
auto registerPage(uWS::App &app, DatabaseManager &db) -> void {
	app.post("/register", [&app, &db](auto *res, auto *req) {
		// Loop through header 
		for (auto header : *req){
			std::cout << header.first << ", " << header.second << "\n";
		};
		
		// Get data from request
		res->onData([&db, res](std::string_view data, bool last) {
			auto buffer = std::string("");
			auto message = std::string("");
			
			buffer.append(data.data(), data.length());
			if (last) {
				auto user_json = json::parse(data);
				auto username = std::string(user_json["username"]);
				auto email = std::string(user_json["email"]);
				auto password = std::string(user_json["password"]);


				// Register Success
				if (db.insert_user(username, email, password)){
					message = "{\"event'\": \"register\", \"action\": \"register\","
					"\"payload\": { \"outcome\" : \"success\"}}";
				}
				// Register Failure
				else{
					message = "{\"event'\": \"register\", \"action\": \"register\", "
						"\"payload\": { \"outcome\" : \"failure\"}}";
				}
				// Respond
				res->end(message);
			}
		});
		res->onAborted([]() -> void {});
	});
}

auto login(uWS::App &app, DatabaseManager &db, std::unordered_map<int, std::string> &session_tokens) -> void {	
	app.post("/login", [&app, &db, &session_tokens](auto *res, auto *req){
		// Get data from request
		res->onData([&db, res, &session_tokens](std::string_view data, bool last) {
			auto buffer = std::string("");
			auto message = std::string("");
			
			buffer.append(data.data(), data.length());
			if (last) {
				auto user_json = json::parse(data);
				auto email = std::string(user_json["email"]);
				auto password = std::string(user_json["password"]); 
		
				auto user = db.get_user(email);

				if (user != NULL){
					// Incorrect password
					if (hash_password(password) != user->password_hash){
						std::cout << "LOGIN: incorrect login\n";
						 message =  "{\"event\": \"login\", \"action\": \"login\", \"payload\":" 
							"{ \"outcome\" : \"failure\", \"message\": \"incorrect password\"}}";
					// Login success
					}else{
						std::cout << "LOGIN: successful login\n";
						if (session_tokens.find(user->id) == session_tokens.end()){
							auto session_token = generate_session_token(user->id);
							session_tokens[user->id] = session_token;
						}
					
						message =  std::string("{\"event\": \"login\", \"action\": \"login\", \"payload\":") +  
							std::string("{ \"outcome\" : \"success\", \"uid\": \"") +  std::to_string(user->id) +
							std::string("\", \"email\": \"") + user->email + std::string("\" ") + ","+
							std::string ("\"session\": \"") + session_tokens[user->id] + std::string("\"}}");
					}
				// Email doesn'e exist
				}else{
					std::cout << "LOGIN: Doesn't exist\n";
					message = "{\"event\": \"login\", \"action\": \"login\", \"payload\" :" 
							"{ \"outcome\": \"failure\", \"message\": \"email not in database\"}}";
				}
				
				res->end(message);
			}
		});
		res->onAborted([]() -> void {});
	});
}

// GET REQUESTS
auto api_profile(uWS::App &app, DatabaseManager &db, std::unordered_map<int, std::string> &session_tokens) -> void {
	app.get("/api/profile", [&app, &db, &session_tokens](auto *res, auto *req) {
	
		// todo, parse request 
		
		// hard coded user for functionality 
		auto user = db.get_user(1);
		auto stats = db.get_stats(1);
		auto friends = db.get_friends(1);
		
		auto profile_json = profile_to_json(user, stats, friends);
				
		std::cout << profile_json << "\n";
		res->end(profile_json);	
	});	
}

// Gets all replays
auto api_replay(uWS::App &app, DatabaseManager &db) -> void {
	app.get("/api/replay", [&app, &db](auto *res, auto *req) {
		auto matchid = std::string(req->getQuery("matchid")); 
		// int id = atoi(matchid);
		// auto match = db.get_matches(id);
		
		json payload;

		res->end(payload.dump());
	});
}

auto api_search_all(uWS::App &app, DatabaseManager &db) -> void {
	app.get("/api/search/all", [&app, &db](auto *res, auto *req) {
		json payload;
		std::string key = "all_matches";
		payload[key] = {};
		
		std::cout << "Getting all matches\n";
		auto matches = db.get_matches();
		std::cout << "Retrieved\n";
		for (auto const& match : matches) {
			payload[key].push_back(match.to_json());
		}

		res->end(payload.dump());
	});
}

auto api_search_snapshot(uWS::App &app, DatabaseManager &db) -> void {
	// Query => /api/search/snapshot?moves=e5a1
	app.get("/api/search/snapshot", [&app, &db](auto *res, auto *req) {
		// for (auto header : *req){
		// 	std::cout << header.first << ", " << header.second << "\n";
		// };

		// Get given snapshot move sequence from frontend get query
		auto moves = std::string(req->getQuery("moves")); 
		auto metadata = MetaDataGenerator(moves, 2); 		// TODO: Hardcoded snapshot search for 2 players only
		auto const positions = metadata.db_snapshot();
		
		// Get all matches that match the given snapshot
		auto nmoves = moves.size()/2;
		auto const& lastpos = positions.end()[-1];
		auto const& priorpos = positions.end()[-2];
		auto matches = db.get_matches(nmoves, lastpos, nmoves - 1, priorpos);

		// Serve the matches to frontend
		json payload;
		std::string key = "snapshot_matches";
		payload[key] = {};
		for (auto const& match : matches) {
			payload[key].push_back(match.to_json());
		}

		res->end(payload.dump());
	});
}

// TESTING ENDPOINTS
auto api_david(uWS::App &app) -> void {
	app.get("/api/david", [&app](auto *res, auto *req) {
		res->end("{\"name\": \"david\"}");
	});
}
auto api_db(uWS::App &app, DatabaseManager &db) -> void {
  app.get("/db", [&db](auto *res, auto *req) {
	// Testing Database Functionality.
		// Inserting new user
		// db.insert_user("new_user", "email", "password");
		// auto user = db.get_user("email");
		// std::cout << user->password_hash << std::endl;
		// Inserting match.
		auto playersELO = std::map<int, int>{
			{1, 1000},
			{2, 1200}
		};
		auto winner = 1;
		auto move_seq = "12345";
		auto snapshots = std::vector<uint64_t>{100, 200, 300, 400, 500};
		auto matchID = db.save_match("POTHOLES", false, playersELO, winner, move_seq, snapshots);
		db.save_match("CLASSIC", false, playersELO, winner, move_seq, snapshots);
		db.save_match("TRIPLES", false, playersELO, winner, move_seq, snapshots);
		auto matches = db.get_matches(1);
		for (auto const &match : matches) {
			std::cout << match.id << " " << match.game << " " <<
				match.replay << std::endl;
			for (auto const &p : match.players) {
				std::cout << p.username << " " << p.end_elo << " " <<
					p.outcome << std::endl;
			}
		}
		// Does user exist.
		std::cout << db.does_user_exist("David") << std::endl;
		std::cout << db.does_user_exist("JackyJ") << std::endl;
		std::cout << db.does_user_exist("DNE") << std::endl;
		// Latest ELO
		std::cout << db.get_latest_elo(1, "CLASSIC") << std::endl;
		std::cout << db.get_latest_elo(5, "CLASSIC") << std::endl;
		// Get stats
		auto stats = db.get_stats(1);
		std::cout << stats->get_WLD("CLASSIC", false).at(0) << std::endl;
		std::cout << stats->get_WLD("TRIPLES", false).at(0) << std::endl;
		std::cout << stats->get_WLD("POTHOLES", false).at(0) << std::endl;
		// Snapshots
		auto m = db.get_matches(4, 400, 5, 500);
		for (auto const &match : m) {
			std::cout << match.id << " " << match.game << " " <<
				match.replay << std::endl;
			for (auto const &p : match.players) {
				std::cout << p.username << " " << p.end_elo << " " <<
					p.outcome << std::endl;
			}
		}
		res->end(std::to_string(matchID));
		
		// testing get friends 
		auto friends = db.get_friends(1);
		auto friends_json = friends_to_json(1, friends);
		std::cout << " ***** friends to json\n";
		std::cout << friends_json;
		std::cout <<  "*****\n";
		
		// testing add friend
		auto add = db.accept_friend_req(1,6);
		if (add){
			std::cout << "add 1 6 true\n";
		}else{
			std::cout << "add 1 6 false\n";

		}
		auto del = db.delete_friend(1,6);
		if (del){
			std::cout << "del 1 6 true\n";
		}else{
			std::cout << "del 1 6 false\n";
		}
		auto del2 = db.delete_friend(1,6);
		if (del2){
			std::cout << "del 1 6 true\n";
		}else{
			std::cout << "del 1 6 false\n";
		}
   });
}