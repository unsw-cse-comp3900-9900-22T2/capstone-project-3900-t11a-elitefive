// Normal includes
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

// External libs
#include <nlohmann/json.hpp>
#include <pqxx/pqxx>
#include "App.h"

// Our libs
#include "board.hpp"
#include "computer.hpp"
#include "db_utils.hpp"
#include "db_manager.hpp"
#include "aigame.hpp"

#include "room.hpp"
#include "metadatagen.hpp"
#include "api.hpp"
#include "server_util.hpp"

using json = nlohmann::json;

void RelaySocket(){
	struct SocketData{
		//Empty because we don't need any currently.
	};
	
	uWS::App app = uWS::App();
	auto database_connection = DatabaseManager();
	auto session_tokens = std::unordered_map<int, std::string>();
	

	auto &db = database_connection;
	auto &tokens = session_tokens;

	app.listen(8080, [](auto *listen_socket){
		if(listen_socket){
			std::cout<< "Listening on port" << 8080 << std::endl;
		};
	});

	// Post Requests
	registerPage(app, db);
	login(app, db, tokens);

	app.get("/api/david", [&app](auto *res, auto *req) {
		res->end("{\"name\": \"david\"}");
	});

	app.get("/api/search/snapshot", [&app, &db](auto *res, auto *req) {
        // for (auto header : *req){
        //     std::cout << header.first << ", " << header.second << "\n";
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

	Room room = Room(app, &db, {1, 2});

	app.run();
}


int main()
{
	RelaySocket();
	return 0;
}
