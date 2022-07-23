#include "api.hpp"
#include "App.h"
#include "db_manager.hpp"
#include "db_utils.hpp"
#include "server_util.hpp"
#include "metadatagen.hpp"

#include <algorithm>
#include <cstdlib>

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
				
				// Manage json
				json payload;
				if (db.insert_user(username, email, password)){
				
					// send varification email
					send_email_varification(email, username);

					
				
					// Register Success
					payload["event"] = "register";
					payload["action"] = "register";
					payload["payload"]["outcome"] = "success";
				}
				else {
					// Register Failure
					payload["event"] = "register";
					payload["action"] = "register";
					payload["payload"]["outcome"] = "failure";
				}
				// Respond
				res->end(payload.dump());
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
				// Manage JSON
				json payload;
				if (user != NULL){
					// Incorrect password
					if (hash_password(password) != user->password_hash){
						std::cout << "LOGIN: incorrect login\n";
						payload["event"] = "login";
						payload["action"] = "login";
						payload["payload"]["outcome"] = "failure";
						payload["payload"]["message"] = "incorrect password";
					} else {
						// Login success
						std::cout << "LOGIN: successful login\n";
						if (session_tokens.find(user->id) == session_tokens.end()){
							auto session_token = generate_session_token(user->id);
							session_tokens[user->id] = session_token;
						}
						payload["event"] = "login";
						payload["action"] = "login";
						payload["payload"]["outcome"] = "success";
						payload["payload"]["uid"] = std::to_string(user->id);
						payload["payload"]["email"] = user->email;
						payload["payload"]["session"] = session_tokens[user->id];
					}
				// Email doesn'e exist
				} else{
					std::cout << "LOGIN: Doesn't exist\n";
					payload["event"] = "login";
					payload["action"] = "login";
					payload["payload"]["outcome"] = "failure";
					payload["payload"]["message"] = "email not in database";
				}
				
				res->end(payload.dump());
			}
		});
		res->onAborted([]() -> void {});
	});
}

auto friendaction(uWS::App &app, DatabaseManager &db, std::unordered_map<int, std::string> &session_tokens) -> void {
	app.post("/friend", [&app, &db, &session_tokens](auto *res, auto *req){
		// Get data from request
		res->onData([&db, res, &session_tokens](std::string_view data, bool last) {
			auto buffer = std::string("");
			auto message = std::string("");
			buffer.append(data.data(), data.length());
			if (last) {
				auto data_json = json::parse(data);
				auto from = std::string(data_json["from"]);
				auto to = std::string(data_json["to"]);
				auto action = std::string(data_json["action"]);
				auto from_id = stoi(from);
				
				// Manage JSON
				json payload;
				payload["event"] = "friend";
				payload["action"] = action;
				if (action == "add") {
					auto to_user = db.get_user_username(to);
					if (to_user == nullptr){
						// If username does not exist.
						std::cout << "ADD FRIEND: Username does not exist" << std::endl;
						payload["payload"]["outcome"] = "failure";
						payload["payload"]["message"] = "username does not exist";
					} else {
						// Exists. Check if already friends.
						if (db.are_friends(from_id, to_user->id)) {
							payload["payload"]["outcome"] = "failure";
							payload["payload"]["message"] = "already friends";
						} else {
							// Add friend.
							bool success = db.send_friend_req(from_id, to_user->id);
							if (success) {
								payload["payload"]["outcome"] = "success";
								payload["payload"]["to_id"] = to_user->id;
								payload["payload"]["to_username"] = to_user->username;
							} else {
								payload["payload"]["outcome"] = "failure";
								payload["payload"]["message"] = "a problem occurred";
							}
						}
					}
				} else {
					bool success = false;
					auto to_id = stoi(to);
					auto to_user = db.get_user(to_id);
					if (action == "accept") {
						success = db.accept_friend_req(from_id, to_id);
					} else if (action == "deny") {
						success = db.deny_friend_req(from_id, to_id);
					} else if (action == "delete") {
						success = db.delete_friend(from_id, to_id);
					} else if (action == "cancel") {
						success = db.revoke_friend_req(from_id, to_id);
					}
					// Check if successful.
					if (success) {
						payload["payload"]["outcome"] = "success";
						payload["payload"]["to_id"] = to_user->id;
						payload["payload"]["to_username"] = to_user->username;
					} else {
						payload["payload"]["outcome"] = "failure";
						payload["payload"]["message"] = "a problem occurred";
					}
				}
				res->end(payload.dump());
			}
		});
		res->onAborted([]() -> void {});
	});
}

// GET REQUESTS
auto api_profile(uWS::App &app, DatabaseManager &db, std::unordered_map<int, std::string> &session_tokens) -> void {
	app.get("/api/profile", [&app, &db, &session_tokens](auto *res, auto *req) {
		auto suid = std::string(req->getQuery("uid")); 
		auto uid = atoi(suid.c_str());

		// hard coded user for functionality 
		auto user = db.get_user(uid);
		auto stats = db.get_stats(uid);
		auto elos = std::map<std::string, int>{
			{"CLASSIC", db.get_latest_elo(uid, "CLASSIC")},
			{"TRIPLES", db.get_latest_elo(uid, "TRIPLES")},
			{"POTHOLES", db.get_latest_elo(uid, "POTHOLES")},
		};
		auto friends = db.get_friends(uid);
		
		json profile_json;
		if (user != nullptr) {
			profile_json = profile_to_json(user, stats, elos, friends);
		}
		res->end(profile_json.dump());	
	});	
}

auto api_replay(uWS::App &app, DatabaseManager &db) -> void {
	app.get("/api/replay", [&app, &db](auto *res, auto *req) {
		auto matchid = std::string(req->getQuery("matchid")); 
		auto id = atoi(matchid.c_str());
		
		// Fetch the match with given ID from database.
		auto match = db.get_match(id);
		json payload;
		if (match != nullptr) {
			payload = match->to_json();

			int nplayers = payload["players"].size();
			std::string const& moves = payload["moves"];

			payload["nplayers"] = nplayers;
			payload["holes"] = {};
						
			auto meta = MetaDataGenerator(moves, nplayers);
			payload["move_seq"] = {};
			for (int player = 0; player < nplayers; ++player) {
				payload["move_seq"][player] = meta.moves_by(player);
			}
		}
		res->end(payload.dump());
	});
}

auto api_friends(uWS::App &app, DatabaseManager &db, std::unordered_map<int, std::string> &session_tokens) -> void {
	app.get("/api/friends", [&app, &db, &session_tokens](auto *res, auto *req) {
		auto suid = std::string(req->getQuery("uid")); 
		auto uid = atoi(suid.c_str());

		auto friends = db.get_friends(uid);
		auto incoming = db.get_incoming_freqs(uid);
		auto outgoing = db.get_outgoing_freqs(uid);
		
		auto friends_json = all_friends_to_json(uid, friends, incoming, outgoing);
				
		res->end(friends_json.dump());	
	});
}

// Gets all replays
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

		// Get given snapshot move sequence from frontend get query
		auto moves = std::string(req->getQuery("moves")); 
		auto metadata = MetaDataGenerator(moves, 2); 		// TODO: Hardcoded snapshot search for 2 players only
		auto const positions = metadata.db_snapshot();
		
		// Get all matches that match the given snapshot
		auto nmoves = moves.size()/2;
		auto const& lastpos = positions.end()[-1];
		auto const& priorpos = positions.end()[-2];
		auto matches = (nmoves <= 1) ? db.get_matches(nmoves, lastpos)
		: db.get_matches(nmoves, lastpos, nmoves - 1, priorpos);

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

// API HELPER FUNCTIONS
auto send_email_varification(std::string email, std::string username) -> void{
	
	auto message = std::string("Hello ") + username + std::string("!! ")
	+ std::string("Welcome to Yavalath. Your email verification code is: ");

	auto cmd = "/app/mail/send_email.sh " + email + " \"Welcome to Yavalath!\" \"" + message + " \"";
	
	std::cout << cmd << "\n" ;
	
	std::system(cmd.c_str());
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
		// auto friends_json = friends_to_json(1, friends);
		// std::cout << " ***** friends to json\n";
		// std::cout << friends_json;
		// std::cout <<  "\n *****\n";
		
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