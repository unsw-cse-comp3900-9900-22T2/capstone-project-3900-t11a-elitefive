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

  app.get("/db", [&db](auto *res, auto *req) {
     // test inserting into db
		//  db.insert_user("username", "email", "password");
		//  auto user = db.get_user("email");
		//  res->end(user->password_hash);
		auto matchID = db.save_match("CLASSIC", "12345");
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
			auto json = nlohmann::json::parse(message);
			std::string datastring = json["data"];
			auto data = nlohmann::json::parse(datastring);
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
					auto const match_id = db.save_match("CLASSIC", game->move_sequence());
					std::cout << "Match ID: " << match_id << '\n';
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
