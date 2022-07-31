#include "api.hpp"
#include "App.h"
#include "db_manager.hpp"
#include "db_utils.hpp"
#include "server_util.hpp"
#include "metadatagen.hpp"

#include <algorithm>
#include <cstdlib>

using json = nlohmann::ordered_json;

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
				
				json payload;
				if (password.length() < 8 ){
					// Register Failure
					payload["event"] = "register";
					payload["action"] = "register";
					payload["payload"]["outcome"] = "failure";
					payload["payload"]["message"] = "password must be at least 8 characters";
				}
				else if (db.insert_user(username, email, password)){
				
					// send welcome email
					send_email_welcome(email, username);
					
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
					payload["payload"]["message"] = "email already exists";

				}
				// Respond
				res->end(payload.dump());
			}
		});
		res->onAborted([]() -> void {});
	});
}

auto tempPass(uWS::App &app, DatabaseManager &db) -> void {
	app.post("/api/temppass", [&app, &db](auto *res, auto *req) {

		// Get data from request
		res->onData([&db, res](std::string_view data, bool last) {
			auto buffer = std::string("");
			auto message = std::string("");
			
			buffer.append(data.data(), data.length());
			if (last) {
			
				json payload;
				payload["event"] = "password";
				payload["action"] = "temporary";
				
				auto password = generate_temporary_password();
				auto temp_pass_hash = hash_password(password);
			
				// Read message
				auto email_json = json::parse(data);
				std::string email = email_json["email"];
				
				auto user = db.get_user(email);
				if (user == NULL){
					payload["payload"]["outcome"] = "failure";
					payload["payload"]["message"] = "email not on database";
				}
				else if (db.change_password(user->id, temp_pass_hash)){
					send_email_temp_password(user->email, user->username, password);
					payload["payload"]["outcome"] = "success";
				}else{
					payload["payload"]["outcome"] = "failure";
				}
				res->end(payload.dump());
			}
		});
		res->onAborted([]() -> void {});
	});
}

auto resetPass(uWS::App &app, DatabaseManager &db) -> void {
	app.post("/api/resetpassword", [&app, &db](auto *res, auto *req) {

		// Get data from request
		res->onData([&db, res](std::string_view data, bool last) {
			auto buffer = std::string("");
			auto message = std::string("");
			
			buffer.append(data.data(), data.length());
			if (last) {
			
				json payload;
				payload["event"] = "password";
				payload["action"] = "reset";
				
				// Read message
				auto res_json = json::parse(data);
				std::string suid = res_json["uid"];
				auto uid = atoi(suid.c_str());
				std::string oldpass = res_json["oldpass"];
				std::string newpass = res_json["newpass"];
				
				// Check if old password is correct 
				auto user = db.get_user(uid);
				if (hash_password(oldpass) != user->password_hash){
					payload["payload"]["outcome"] = "failure";
					payload["payload"]["message"] = "incorrect password";
				}else if(newpass.length() < 8 ){
					payload["payload"]["outcome"] = "failure";
					payload["payload"]["message"] = "new password must be at least 8 characters";
				}else if (db.change_password(user->id, hash_password(newpass))){
					payload["payload"]["outcome"] = "success";
					payload["payload"]["message"] = "success";
				}else{
					payload["payload"]["outcome"] = "failure";
					payload["payload"]["message"] = "database error";
				}
				
				res->end(payload.dump());
			}
		});
		res->onAborted([]() -> void {});
	});
}

auto changePW(uWS::App &app, DatabaseManager &db) -> void {
	app.post("/api/changepw", [&app, &db](auto *res, auto *req) {
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
				// Read message
				auto user_json = json::parse(data);
				std::string suid = user_json["uid"];
				int uid = atoi(suid.c_str());
				auto password = std::string(user_json["password"]);
				
				auto temp_pass_hash = hash_password(password);
				
				json payload;
				payload["event"] = "password";
				payload["action"] = "change";

				if (db.change_password(uid, temp_pass_hash)){
					auto user = db.get_user(uid);
					send_email_temp_password(user->email, user->username, password);
					payload["payload"]["outcome"] = "success";
				}else{
					payload["payload"]["outcome"] = "failure";
				}
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
						payload["payload"]["username"] = user->username;
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
		auto elohistory = std::map<std::string, std::vector<int>>{
			{"CLASSIC", db.get_elo_progress(uid, "CLASSIC")},
			{"TRIPLES", db.get_elo_progress(uid, "TRIPLES")},
			{"POTHOLES", db.get_elo_progress(uid, "POTHOLES")}
		};
		auto matchhistory = db.get_matches(uid);
		auto friends = db.get_friends(uid);
		auto lastmatch = db.get_last_match(uid);
		
		json profile_json;
		if (user != nullptr) {
			profile_json = profile_to_json(user, stats, elos, elohistory, matchhistory, friends);
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

			// Potholes.
			for (int i = 0; i < match->potholes.size(); i = i + 2) {
				std::string hole = match->potholes.substr(i, 2);
				payload["holes"].push_back(hole);
			}
						
			auto meta = MetaDataGenerator(moves, nplayers);
			// Moves by player.
			payload["move_seq"] = {};
			for (int player = 0; player < nplayers; ++player) {
				payload["move_seq"][player] = meta.moves_by(player);
			}

			// Turn Array.
			auto turn_array = meta.turn_array();
			payload["turns"] = turn_array;
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

auto api_leaderboards(uWS::App &app, DatabaseManager &db) -> void {
	app.get("/api/leaderboards", [&app, &db](auto *res, auto *req) {
		auto suid = std::string(req->getQuery("uid")); 
		auto uid = atoi(suid.c_str());
		// Generate the leaderboards json.
		auto g_classic = db.get_global_leaderboard("CLASSIC");
		auto g_triples = db.get_global_leaderboard("TRIPLES");
		auto g_pothole = db.get_global_leaderboard("POTHOLES");
		auto f_classic = db.get_friend_leaderboard("CLASSIC", uid);
		auto f_triples = db.get_friend_leaderboard("TRIPLES", uid);
		auto f_pothole = db.get_friend_leaderboard("POTHOLES", uid);
		json leaderboards;
		leaderboards["global_leaderboard"]["classic"] = {};
		leaderboards["global_leaderboard"]["triples"] = {};
		leaderboards["global_leaderboard"]["potholes"] = {};
		leaderboards["friend_leaderboard"]["classic"] = {};
		leaderboards["friend_leaderboard"]["triples"] = {};
		leaderboards["friend_leaderboard"]["potholes"] = {};
		for (auto const &entry : g_classic) {
			leaderboards["global_leaderboard"]["classic"].push_back(entry.to_json());
		}
		for (auto const &entry : g_triples) {
			leaderboards["global_leaderboard"]["triples"].push_back(entry.to_json());
		}
		for (auto const &entry : g_pothole) {
			leaderboards["global_leaderboard"]["potholes"].push_back(entry.to_json());
		}
		for (auto const &entry : f_classic) {
			leaderboards["friend_leaderboard"]["classic"].push_back(entry.to_json());
		}
		for (auto const &entry : f_triples) {
			leaderboards["friend_leaderboard"]["triples"].push_back(entry.to_json());
		}
		for (auto const &entry : f_pothole) {
			leaderboards["friend_leaderboard"]["potholes"].push_back(entry.to_json());
		}
		res->end(leaderboards.dump());
	});
}

auto api_social_feed(uWS::App &app, DatabaseManager &db) -> void {
	app.get("/api/social/feed", [&app, &db](auto *res, auto *req) {
		auto suid = std::string(req->getQuery("uid")); 
		auto uid = atoi(suid.c_str());
		auto gamemodes = std::vector<std::string>{"CLASSIC", "TRIPLES", "POTHOLES"};
		// Generate the social feed JSON.
		json feed;
		// Get the user's friends.
		auto friends = db.get_friends(uid);
		if (friends.empty()) {
			// If no friends, suggest friends.
			feed.push_back(social_json("", "Add friends to receive more social feed news!"));
		} else {
			// 1. Random friend's random match.
			auto friend1 = friends.at(random_num(friends.size()));
			auto matches = db.get_matches(friend1->id);
			auto match = matches.at(random_num(matches.size()));
			feed.push_back(social_json("/replay/" + std::to_string(match->id),
				"Click here to watch " + friend1->username + " play a " + match->game + " match!"));
			// 2. Random friend's latest elo.
			auto friend2 = friends.at(random_num(friends.size()));
			auto gamemode2 = gamemodes.at(random_num(gamemodes.size()));
			auto recent_elo = db.get_latest_elo(friend2->id, gamemode2);
			feed.push_back(social_json("/profile/" + std::to_string(friend2->id),
				friend2->username + " recently reached " + std::to_string(recent_elo) + " in " + gamemode2 +
				" mode! (Click to view!)"));
			// 3. Random friend's friend leaderboard ranking OR global rank for gamemode.
			auto friend3 = friends.at(random_num(friends.size()));
			auto gamemode3 = gamemodes.at(random_num(gamemodes.size()));
			auto f_lb = db.get_friend_leaderboard(gamemode3, uid);
			auto rank = db.get_global_rank(gamemode3, friend3->id);
			if (rank != -1) {
				feed.push_back(social_json("/leaderboard",
					friend3->username + " is ranked #" + std::to_string(rank) + " on the global " + gamemode3 +
					" leaderboard! (Click to view!)"));	
			} else if (!f_lb.empty()) {
				auto lb_entry = f_lb.at(random_num(f_lb.size()));
				feed.push_back(social_json("/leaderboard",
					lb_entry.username + " is ranked #" + std::to_string(lb_entry.rank) + " on your friend " + gamemode3 +
					" leaderboard! (Click to view!)"));	
			}
		}
		// Recommend a gamemode.
		feed.push_back(social_json("", "You should try out the "
			+ gamemodes.at(random_num(gamemodes.size())) + " gamemode!"));
		// Get player's last match.
		auto lastmatch = db.get_last_match(uid);
		if (lastmatch != nullptr) {
			feed.push_back(social_json("/replay/" + std::to_string(lastmatch->id),
				"Click here to replay your last match!"));
		} else {
			feed.push_back(social_json("", "Looks like you haven't played a match yet... You should play one!"));
		}
		res->end(feed.dump());
	});
}

// Gets all replays
auto api_search_all(uWS::App &app, DatabaseManager &db) -> void {
	app.get("/api/search/all", [&app, &db](auto *res, auto *req) {
		auto filter = std::string(req->getQuery("filter"));
		auto value = std::string(req->getQuery("value"));
		json payload;
		payload["all_matches"] = {};
		payload["classic"] = {};
		payload["triples"] = {};
		payload["potholes"] = {};
		// Filter.
		std::function<bool(Match*)> fcheck = [](Match* m) -> bool { return true; };
		if (filter == "type") {
			if (value == "ranked") {
				fcheck = [](Match* m) -> bool { return m->ranked; };
			} else if (value == "unranked") {
				fcheck = [](Match* m) -> bool { return !m->ranked; };
			}
		} else if (filter == "game") {
			if (value == "classic") {
				fcheck = [](Match* m) -> bool { return m->game == "CLASSIC"; };
			} else if (value == "triples") {
				fcheck = [](Match* m) -> bool { return m->game == "TRIPLES"; };
			} else if (value == "potholes") {
				fcheck = [](Match* m) -> bool { return m->game == "POTHOLES"; };
			}
		} else if (filter == "player") {
			fcheck = [value](Match* m) -> bool {
				for (auto const &p : m->players) {
					if (p.username == value) {
						return true;
					}
				}
				return false;
			};
		}
		// All matches...
		auto matches = db.get_matches();
		for (auto const& match : matches) {
			if (fcheck(match)) {
				// Push to mode-specific list.
				auto game = match->game;
				std::transform(game.begin(), game.end(), game.begin(),
					[](unsigned char c) { return std::tolower(c); });
				payload[game].push_back(match->to_json());
				// Push back to all matches.
				payload["all_matches"].push_back(match->to_json());
			}
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
			payload[key].push_back(match->to_json());
		}

		res->end(payload.dump());
	});
}

/*
auto api_resetpass(uWS::App &app, DatabaseManager &db) -> void {
	app.get("/api/resetpassword", [&app, &db](auto *res, auto *req) {
	
		std::cout << "reset pass\n\n";
	
		auto suid = std::string(req->getQuery("uid")); 
		auto uid = atoi(suid.c_str());
		auto oldpass = std::string(req->getQuery("oldpass")); 
		auto newpass = std::string(req->getQuery("newpass")); 
		
		auto new_pass_hash = hash_password(newpass);
		
		json payload;
		payload["event"] = "password";
		payload["action"] = "reset";

		
		if (db.change_password(uid, new_pass_hash)){
			payload["payload"]["outcome"] = "success";
		}else{
			payload["payload"]["outcome"] = "failure";

		}
		
		res->end(payload.dump());
	});
}
*/



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
		auto potholes = "";
		auto move_seq = "12345";
		auto svg_data = "";
		auto snapshots = std::vector<uint64_t>{100, 200, 300, 400, 500};
		auto matchID = db.save_match("POTHOLES", false, playersELO, winner, potholes, move_seq, svg_data, snapshots);
		db.save_match("CLASSIC", false, playersELO, winner, potholes, move_seq, svg_data, snapshots);
		db.save_match("TRIPLES", false, playersELO, winner, potholes, move_seq, svg_data, snapshots);
		auto matches = db.get_matches(1);
		for (auto const &match : matches) {
			std::cout << match->id << " " << match->game << " " <<
				match->replay << std::endl;
			for (auto const &p : match->players) {
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
			std::cout << match->id << " " << match->game << " " <<
				match->replay << std::endl;
			for (auto const &p : match->players) {
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