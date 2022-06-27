#include <iostream>
#include <uwebsockets/App.h>
#include <uwebsockets/WebSocket.h>
#include <libusockets.h>
// #include <uwebsockets/HttpContext.h>
// #include <uwebsockets/AsyncSocket.h>
#include <memory>
#include "src/board.hpp"
#include "db_functions.hpp"
// #include <pqxx/pqxx>

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
			ws->subscribe("ROOM1");
			board = std::make_unique<Board>(2, std::vector<int>{100,0});
		},
		.message = [&board](auto *ws, std::string_view message, uWS::OpCode opCode){
			std::cout << message << '\n';
			board->play_move(std::string("a3"));
			ws->publish("ROOM1", "{\"event\": \"move\", \"tile\": \"a3\"}", opCode);
			std::cout << *board << '\n';
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
