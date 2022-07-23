#include <nlohmann/json.hpp>

#include "pool.hpp"
#include "room.hpp"
#include "db_manager.hpp"

#include <string>
#include <queue>
#include <unistd.h>
#include <stdio.h>

using json = nlohmann::json;

auto game_selection(int opponent_uid, int uid, std::string room_id, std::string gamemode, bool ranked_flag, bool ai_flag) -> json;

struct SocketData{
	//Empty because we don't need any currently.
};

auto convert_to_bool(std::string const& key) -> bool {
	if (key == "true") return true;
	if (key == "false") return false;
	std::cout << "DATA FROM FRONTEND IS INCORRECT\n";
	exit(1);
}

Pool::Pool(uWS::App &app, DatabaseManager *db, std::vector<Room *> &rooms)
: room_id_{"WAITING_ROOM"}
{
	create_waiting_room(app, db, rooms);
}

auto game_selection(int opponent_uid, int uid, std::string room_id, std::string gamemode, bool ranked_flag, bool ai_flag) -> json {
	json payload;
	payload["event"] = "match_created";
	payload["uids"] = {opponent_uid, uid};
	payload["room_id"] = room_id;
	payload["gamemode"] = gamemode;
	payload["elo"] = ranked_flag;
	payload["ai"] = ai_flag;
	return payload;
}

auto Pool::create_waiting_room(uWS::App &app, DatabaseManager *db, std::vector<Room *> &rooms) -> void {
	auto publish = [this](auto *ws, std::string message, uWS::OpCode opCode) -> void {
		ws->publish(this->room_id(), message, opCode);
		ws->send(message, opCode);
	};
	printf("Pointer: %p\n", &rooms);
	printf("DB Pointer: %p\n", db);

	app.ws<SocketData>("/ws/waitingroom",uWS::TemplatedApp<false>::WebSocketBehavior<SocketData> {
		.open = [this, publish](auto *ws) {
			ws->subscribe(this->room_id());
			std::cout << "\tPool: Joined room\n";
		},
		.message = [this, publish, db, &app, &rooms](auto *ws, std::string_view message, uWS::OpCode opCode) mutable {
			std::cout << "\tPool: Recieved message\n";
			
			// NEED TO REPLACE WITH PROPER FRONTEND
			auto json_data = nlohmann::json::parse(message);
			std::string datastring = json_data["data"];
			auto data = nlohmann::json::parse(datastring);

			std::cout << "\t\tPool: Data Recieved - "  << data << '\n'; 
			
			// Parse data
			std::string suid = data["uid"];
			int uid = atoi(suid.c_str());

			// TODO: UNCOMMENT THIS OUT TO USE PROVIDED FRONTEND DATA
			bool ranked_flag = data["ai"];
			bool ai_flag = data["ranked"];
			std::string gamemode = "CLASSIC";

			// TODO: DELETE THESE HARDCODED VALUES AND USE ABOVE JSON
			// std::cout << "\n\n\t\t=== WARNING ===\nBACKEND NOT READING FRONTEND JSON. GO IN CODE AND CHANGE!\n\t\t=== WARNING ===\n\n";
			// std::string s_ranked = "false";
			// std::string s_ai = "true";

			// bool ranked_flag = convert_to_bool(s_ranked);
			// bool ai_flag = convert_to_bool(s_ai);

			// Put them into waiting lobby
			if (ai_flag == false) {
				// Ignore player if they are already in queue
				if (player_waiting_in_classic(uid)) {
					std::cout << "\tPool: Already waiting in queue\n";
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
							delete room;
							room = nullptr;
							// TODO: Properly erase from vector
							break;
						}
					}
					printf("DB Pointer: %p\n", db);
					// false indicated human vs human instead of ai
					rooms.push_back(new Room(app, db, ranked_flag, ai_flag, std::to_string(room_id), {opponent, uid}));
					std::cout << "Created room: " << std::to_string(room_id) << " Opp: " << opponent << " Self: " << uid << '\n'; 
					// printf("Pointer: %p\n", room);

					
					json payload = game_selection(opponent, uid, std::to_string(room_id), gamemode, ranked_flag, ai_flag);
					std::cout << payload.dump() << '\n';
				}
			}
			else { // Versing AI
				int computer_uid = 6; // TODO: HARDCODED
				uint32_t room_id = ((uint32_t) uid << 17) | (uint32_t) computer_uid;
				
				for (auto &room : rooms) {
					if (room == nullptr) continue;
					if (room->room_id() == std::to_string(room_id)) {
						delete room;
						room = nullptr;
						// TODO: Properly erase from vector
						break;
					}
				}
				
				rooms.push_back(new Room(app, db, ranked_flag, ai_flag, std::to_string(room_id), {uid, computer_uid}));
		
				json payload;
				payload["event"] = "match_created";
				payload["uids"] = {uid, computer_uid};
				payload["room_id"] = room_id;
				payload["gamemode"] = "CLASSIC";
				payload["elo"] = ranked_flag;
				payload["ai"] = ai_flag;
				publish(ws, payload.dump(), opCode);
				std::cout << payload.dump() << '\n';
			}
		},
		.close = [this](auto *ws, int x , std::string_view str) {
			ws->unsubscribe(this->room_id());
			std::cout << "\tPool: Player left waiting room\n";
		}
	});
}

auto Pool::room_id() const -> std::string {
	return room_id_;
}

