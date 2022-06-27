#include <iostream>
#include <uwebsockets/App.h>
#include <uwebsockets/WebSocket.h>
#include <libusockets.h>
// #include <uwebsockets/HttpContext.h>
// #include <uwebsockets/AsyncSocket.h>
#include <memory>
#include <string>
#include "src/board.hpp"
#include "db_functions.hpp"
// #include <pqxx/pqxx>
#include <nlohmann/json.hpp>

void RelaySocket(){
	struct SocketData{
		//Empty because we don't need any currently.
	};
	
	auto app = uWS::App();
	
	app.listen(8080, [](auto *listen_socket){
		if(listen_socket){
			std::cout<< "Listening on port" << 8080<< std::endl;
		};
	});

	app.get("/api/david", [&app](auto *res, auto *req) {
		res->end("{\"name\": \"david\"}");
	});

	// app.get("/db", [](auto *res, auto *req) {
  //   // test inserting into db
  //   db_insert_user("jessie" ,"jessie@jessie.com", "meowth");
  //   res->end("pgg");
  // })


	// Functions we have available to in socketing
	// ws->isSubscribed("moves") // See who is subbed to the "moves" room
	// ws->getRemoteAddressAsText() // Gets address of the person
	// ws->send("Hello there boi"); // Option to send message back to client only
	std::unique_ptr<Board> board = nullptr;

	auto websocket = app.ws<SocketData>("/ws/david",uWS::TemplatedApp<false>::WebSocketBehavior<SocketData> {
		.open = [&board](auto *ws) {
			// When client connects, subscribe them to the 'moves' notification board
			ws->subscribe("moves");
			board = std::make_unique<Board>(2, std::vector<int>{100,0});
		},
		.message = [&board](auto *ws, std::string_view message, uWS::OpCode opCode){
			// 1. Parsing JSON to update board backend
			auto json = nlohmann::json::parse(message);
			std::string datastring = json["data"];
			auto data = nlohmann::json::parse(datastring);
			std::string move = data["move"];
			board->play_move(move);

			// 2. AI Move
			// Replace next line with AI move function call.
			std::string ai_move = "b3";
			ws->publish("moves", "{\"event\": \"move\", \"tile\": \"" + ai_move + "\"}", opCode);
			std::cout << *board << '\n';
			// 3. If the game's over, publish game end
			// if () {

			// }
			// ws->publish("end", );
		},
		.close = [](auto *ws, int x , std::string_view str) {
			ws->unsubscribe("moves");
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
