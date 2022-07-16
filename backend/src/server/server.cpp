#include <iostream>
// #include <uwebsockets/App.h>
// #include <uwebsockets/WebSocket.h>
#include "App.h"
// #include <libusockets.h>
#include <nlohmann/json.hpp>

#include <pqxx/pqxx>
#include <memory>
#include <string>

// TODO - Change the relative paths... Not really that good
// https://stackoverflow.com/questions/8304190/cmake-with-include-and-source-paths-basic-setup
#include "board.hpp"
#include "computer.hpp"
#include "db_utils.hpp"
#include "db_manager.hpp"
#include "aigame.hpp"

using json = nlohmann::json;

void RelaySocket(){
	struct SocketData{
		//Empty because we don't need any currently.
	};
	
	auto app = uWS::App();
	auto db = DatabaseManager();
	
	app.listen(8080, [](auto *listen_socket){
		if(listen_socket){
			std::cout<< "Listening on port" << 8080 << std::endl;
		};
	});

	app.get("/api/david", [&app](auto *res, auto *req) {
		res->end("{\"name\": \"david\"}");
	});

	app.post("/register", [&app, &db](auto *res, auto *req){
    
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
    
	app.post("/login", [&app, &db](auto *res, auto *req){
	
	    // Get data from request
	    res->onData([&db, res](std::string_view data, bool last) {
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
                         message =  "{\"event\": \"login\", \"action\": \"login\", \"payload\":" 
                            "{ \"outcome\" : \"failure\", \"message\": \"incorrect password\"}}";
                    // Login success
                    }else{
                        message =  std::string("{\"event\": \"login\", \"action\": \"login\", \"payload\":") +  
                            std::string("{ \"outcome\" : \"success\", \"uid\": \"") +  std::to_string(user->id) +
                            std::string("\", \"email\": \"") + user->email + std::string("\" ") + ","+
                            std::string ("\"session\": \"TODO\"}}");
                    }
				// Email doesn'e exist
                }else{
                    message = "{\"event\": \"login\", \"action\": \"login\", \"payload\" :" 
                            "{ \"outcome\": \"failure\", \"message\": \"email not in database\"}}";
                }
                
                res->end(message);
			}
		});
		res->onAborted([]() -> void {});
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
   });


	// Functions we have available to in socketing
	// ws->isSubscribed("moves") // See who is subbed to the "moves" room
	// ws->getRemoteAddressAsText() // Gets address of the person
	// ws->send("Hello there boi"); // Option to send message back to client only
	std::unique_ptr<Game> game = nullptr;
	std::unique_ptr<AIGame> aigame = nullptr;


	auto websocket = app.ws<SocketData>("/ws/david",uWS::TemplatedApp<false>::WebSocketBehavior<SocketData> {
		.open = [&game, &aigame](auto *ws) {
			// When client connects, subscribe them to the 'moves' notification game
			ws->subscribe("ROOM1");
			game = std::make_unique<Game>(2);
			aigame = std::make_unique<AIGame>(2);
		},
		.message = [&game, &aigame, &db](auto *ws, std::string_view message, uWS::OpCode opCode){
			// 1. Parsing JSON to update board backend
			auto json = json::parse(message);
			std::string datastring = json["data"];
			auto data = json::parse(datastring);
			std::string move = data["move"];
			if (!game->play(move)) {
				return;
			}
			ws->publish("ROOM1", "{\"event\": \"moveconfirm\", \"tile\": \"" + move + "\"}", opCode);

			// TODO: NEED TO MAKE THIS ASYNC SOMEHOW BECAUSE ALL ws->publish in function GETS BATCHED AND SENT AT ONCE
			// 2. AI Move
			// AI/Computer generate move and publish.
			auto computer = Computer(*game);
			if (game->status() == Game::state::ONGOING) {
				// auto move = computer.make_random_move();
				// game->play(move);
				// TODO: I THINK THIS CRASHES IF THERES NO MOVES LEFT?
				aigame->play(move);
				auto const calc_move = aigame->minmax(3); // depth 3 (could increase but test with that)
				game->play(calc_move);
				aigame->play(calc_move);
				aigame->clear();
				std::string ai_move = Game::indexToCoord(calc_move);
				ws->publish("ROOM1", "{\"event\": \"move\", \"tile\": \"" + ai_move + "\"}", opCode);
			}
			std::cout << *game << '\n';
			// 3. If the game's over, publish game end
			Game::state state = game->status();
			if (state != Game::state::ONGOING) {
				std::string winner;
				if (state == Game::state::DRAW) {
					winner = "";
				} else if (game->whose_turn() == 1 && state == Game::state::WIN
						|| game->whose_turn() == 0 && state == Game::state::LOSS) {
					winner = "COMPUTER";
				} else {
					winner = "PLAYER";
				}
				std::cout << *game << '\n';
				// 3. If the game's over, publish game end
				Game::state state = game->status();
				if (state != Game::state::ONGOING) {
					std::string winner;
					if (state == Game::state::DRAW) {
						winner = "";
					} else if (game->whose_turn() == 1 && state == Game::state::WIN
							|| game->whose_turn() == 0 && state == Game::state::LOSS) {
						winner = "COMPUTER";
					} else {
						winner = "PLAYER";
					}
					std::cout << game->move_sequence() << '\n';
					ws->publish("ROOM1", "{\"event\": \"game_over\", \"winner\": \"" + winner + "\"}", opCode);
					// auto const match_id = db.save_match("CLASSIC", game->move_sequence());
					// std::cout << "Match ID: " << match_id << '\n';
				}	
			}
		},
		.close = [](auto *ws, int x , std::string_view str) {
			ws->unsubscribe("ROOM1");
			ws->close();
		}
	});

	app.run();
}

int main()
{
	RelaySocket();
	return 0;
}
