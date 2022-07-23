#include <nlohmann/json.hpp>

#include "pool.hpp"
#include "room.hpp"
#include "db_manager.hpp"

#include <string>
#include <queue>
#include <unistd.h>
#include <stdio.h>

using json = nlohmann::json;

// Helper functions
auto game_selection(int opponent_uid, int uid, std::string room_id, std::string gamemode, bool ranked_flag, bool ai_flag) -> json;
auto parse_pool_data(std::string_view const& message) -> json;

auto Pool::replace_room_id(uint32_t roomid) -> void {
		// printf("Pointer: %p\n", room);
		for (auto &room : rooms) {
			if (room == nullptr) continue;
			if (room->room_id() == std::to_string(roomid)) {
				delete room;
				room = nullptr;
				// TODO: Properly erase from vector
				break;
			}
		}
}


auto Pool::start_player_vs_player_game(std::string const& gamemode, bool ranked_flag, int uid) -> json {
	// We can make a pairing
	// Player 0: Opponent (First to ready up)
	// Player 1: Current  (current person to trigger this)
	int opponent = classic_.front();
	classic_.pop_front();

	uint32_t room_id = ((uint32_t) opponent << 16) | (uint32_t) uid;
	// {uid, computer_uid}
	create_new_room(room_id, ranked_flag, false, {opponent, uid});


	std::cout << "Created room: " << std::to_string(room_id) << " Opp: " << opponent << " Self: " << uid << '\n'; 
	// printf("Pointer: %p\n", room);

	
	json payload = game_selection(opponent, uid, std::to_string(room_id), gamemode, ranked_flag, false);
	std::cout << payload.dump() << '\n';
	return payload;
}



auto Pool::player_vs_player_waiting_lobby(std::string const& gamemode, bool ranked_flag, int uid) -> json {
	// TODO: Do gamemode flags
	// Ignore player if they are already in queue
	if (player_waiting_in_classic(uid)) {
		std::cout << "\tPool: Already waiting in queue\n";
		return {};
	}

	if (players_waiting_classic() < 1) {
		wait_for_classic(uid);
		return {};
	}
	else {
		return start_player_vs_player_game(gamemode, ranked_flag, uid);
	}
}

auto Pool::start_player_vs_ai_game(std::string const& gamemode, bool ranked_flag, int uid) -> json {
	// Versing AI
	int computer_uid = 6; // TODO: HARDCODED AI VALUE
	uint32_t room_id = ((uint32_t) uid << 17) | (uint32_t) computer_uid;
	create_new_room(room_id, ranked_flag, true, {uid, computer_uid});

	json payload;
	payload["event"] = "match_created";
	payload["uids"] = {uid, computer_uid};
	payload["room_id"] = room_id;
	payload["gamemode"] = gamemode;
	payload["elo"] = ranked_flag;
	payload["ai"] = true;
	std::cout << payload.dump() << '\n';
	return payload;
}

auto parse_pool_data(std::string_view const& message) -> json {
	auto json_data = nlohmann::json::parse(message);
	std::string datastring = json_data["data"];
	json data = nlohmann::json::parse(datastring);
	std::cout << "\t\tPool: Data Recieved - "  << data << '\n'; 
	return data;
}

struct SocketData{
	//Empty because we don't need any currently.
};

Pool::Pool(uWS::App *app, DatabaseManager *db)
: room_id_{"WAITING_ROOM"}
, classic_{}
, db{db}
, app{app}
, rooms{}
{
	create_waiting_room();
}

auto Pool::create_waiting_room() -> void {
	auto publish = [this](auto *ws, std::string message, uWS::OpCode opCode) -> void {
		ws->publish(this->room_id(), message, opCode);
		ws->send(message, opCode);
	};
	// printf("Pointer: %p\n", &rooms);
	// printf("DB Pointer: %p\n", db);

	app->ws<SocketData>("/ws/waitingroom",uWS::TemplatedApp<false>::WebSocketBehavior<SocketData> {
		.open = [this, publish](auto *ws) {
			ws->subscribe(this->room_id());
			std::cout << "\tPool: Joined room\n";
		},
		.message = [this, publish](auto *ws, std::string_view message, uWS::OpCode opCode) mutable {
			std::cout << "\tPool: Recieved message\n";
			
			// Parse data
			json data = parse_pool_data(message);
			// TODO: FIX on frontend
			bool ranked_flag = data["ai"];
			bool ai_flag = data["ranked"];
			std::string gamemode = "CLASSIC";	// TODO: Need information from frontend
			std::string suid = data["uid"];
			int uid = atoi(suid.c_str());


			if (ai_flag == false) {
				std::cout << "\tPool: User selected pvp\n";
				json payload = player_vs_player_waiting_lobby(gamemode, ranked_flag, uid);
				if (!payload.empty()) {
					publish(ws, payload.dump(), opCode);	// Put both players into waiting lobby
				}
			}
			else { 
				json payload = start_player_vs_ai_game(gamemode, ranked_flag, uid);
				publish(ws, payload.dump(), opCode);
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
