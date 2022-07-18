#include <nlohmann/json.hpp>

#include "pool.hpp"
#include "room.hpp"
#include "db_manager.hpp"

#include <string>
#include <queue>
#include <unistd.h>
#include <stdio.h>

using json = nlohmann::json;

struct SocketData{
	//Empty because we don't need any currently.
};

Pool::Pool(uWS::App &app, DatabaseManager *db, std::vector<Room *> &rooms)
: room_id_{"WAITING_ROOM"}
{
	create_waiting_room(app, db, rooms);
}

auto Pool::create_waiting_room(uWS::App &app, DatabaseManager *db, std::vector<Room *> &rooms) -> void {
	auto publish = [this](auto *ws, std::string message, uWS::OpCode opCode) -> void {
		ws->publish(this->room_id(), message, opCode);
		ws->send(message, opCode);
	};
	printf("Pointer: %p\n", &rooms);
	app.ws<SocketData>("/ws/waitingroom",uWS::TemplatedApp<false>::WebSocketBehavior<SocketData> {
		.open = [this, publish](auto *ws) {
			ws->subscribe(this->room_id());
			std::cout << "Joined room\n";
			// ws->publish(this->room_id(), "ALJKSDLKJA", uWS::OpCode{200});
		},
		.message = [this, publish, &db, &app, &rooms](auto *ws, std::string_view message, uWS::OpCode opCode) mutable {
			std::cout << "Recieved message\n";
			
			// NEED TO REPLACE WITH PROPER FRONTEND
			auto json_data = nlohmann::json::parse(message);
			std::string datastring = json_data["data"];
			auto data = nlohmann::json::parse(datastring);
			// std::cout << data["move"] << '\n';
			// std::string move = data["move"];
			std::string suid = data["uid"];
			int uid = atoi(suid.c_str());

			// int uid = 0;
			// if (move == "a1") uid = 1;
			// if (move == "a2") uid = 2;
			// if (move == "a3") uid = 3;
			// if (move == "a4") uid = 4;
			
			std::cout << "Player joined: " << uid << '\n';
			// Ignore player if they are already in queue
			if (player_waiting_in_classic(uid)) {
				std::cout << "Already waiting in queue\n";
				return;
			}
			if (players_waiting_classic() < 1) {
				wait_for_classic(uid);
			}
			else {
				// We can make a pairing
				// Player 0: Opponent (First to ready up)
				// Player 1: Current  (current person to trigger this)
				int opponent = classic_.front();
				classic_.pop_front();


				uint32_t room_id = ((uint32_t) opponent << 16) | (uint32_t) uid;
				// printf("Pointer: %p\n", room);
				for (auto &room : rooms) {
					if (room == nullptr) continue;
					if (room->room_id() == std::to_string(room_id)) {
						std::cout << "Room is not null\n";
						delete room;
						room = nullptr;
						// TODO: Properly erase from vector
						break;
					}
				}
				printf("Pointer: %p\n", &rooms);
				rooms.push_back(new Room(app, db, std::to_string(room_id), {opponent, uid}));
				// printf("Pointer: %p\n", room);

				
				json payload;
				payload["event"] = "match_created";
				payload["uids"] = {opponent, uid};
				payload["room_id"] = room_id;
				publish(ws, payload.dump(), opCode);
				std::cout << payload.dump() << '\n';
			}



		},
		.close = [this](auto *ws, int x , std::string_view str) {
			ws->unsubscribe(this->room_id());
			// ws->close();
			std::cout << "Player left waiting room\n";
		}
	});
}

auto Pool::room_id() const -> std::string {
	return room_id_;
}

