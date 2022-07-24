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

// Helper functions
auto make_json_game_selection(std::vector<int> uids, std::string room_id, std::string gamemode, bool ranked_flag, bool ai_flag) -> json;
auto parse_pool_data(std::string_view const& message) -> json;

Pool::Pool(uWS::App *app, DatabaseManager *db)
: room_id_{"WAITING_ROOM"}
, classic_{}
, db{db}
, app{app}
, rooms{}
{
	auto publish = [this](auto *ws, std::string message, uWS::OpCode opCode) -> void {
		ws->publish(this->room_id(), message, opCode);
		ws->send(message, opCode);
	};

	app->ws<SocketData>("/ws/waitingroom",uWS::TemplatedApp<false>::WebSocketBehavior<SocketData> {
		.open = [this, publish](auto *ws) {
			// std::cout << "\tPool: Joined room\n";
			ws->subscribe(this->room_id());
		},
		.message = [this, publish](auto *ws, std::string_view message, uWS::OpCode opCode) mutable {
			// std::cout << "\tPool: Recieved message\n";
			
			// Parse data
			json data = parse_pool_data(message);
			
			std::string gamemode = "TRIPLES";	// TODO: Need information from frontend
			bool ranked_flag = data["ai"];
			bool ai_flag = data["ranked"];

			// Player who is selecting option
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

// ==== Functions to manage the waiting room ====

auto Pool::replace_room_id(uint32_t roomid) -> void {
	for (auto &room : rooms) {
		if (room == nullptr) continue;
		if (room->room_id() == std::to_string(roomid)) {
			delete room;
			room = nullptr;  // TODO: Should properly erase from vector instead of setting to null
			break;
		}
	}
}

auto Pool::start_player_vs_player_game(std::string const& gamemode, bool ranked_flag, int uid) -> json {
	// We can make a pairing // Player 0: Opponent (First to ready up) // Player 1: Current  (current person to trigger this)
	
	if (gamemode == "CLASSIC") {
		// Grab from Queue
		int opponent = classic_.front();
		classic_.pop_front();

		uint32_t room_id = ((uint32_t) opponent << 16) | (uint32_t) uid;
		create_new_room(room_id, ranked_flag, false, {opponent, uid});
		// std::cout << "Created room: " << std::to_string(room_id) << " Opp: " << opponent << " Self: " << uid << '\n'; 
		
		json payload = make_json_game_selection({opponent, uid}, std::to_string(room_id), gamemode, ranked_flag, false);
		return payload;
	}
	else if (gamemode == "TRIPLES") {
		int opp1 = triples_.front();
		triples_.pop_front();
		int opp2 = triples_.front();
		triples_.pop_front();

		uint32_t room_id = ((uint32_t) opp1 << 24) | ((uint32_t) opp2 << 16) | uid;
		create_new_room(room_id, ranked_flag, false, {opp1, opp2, uid});
		json payload = make_json_game_selection({opp1, opp2, uid}, std::to_string(room_id), gamemode, ranked_flag, false);
		return payload;
	}
}

auto Pool::player_vs_player_waiting_lobby(std::string const& gamemode, bool ranked_flag, int uid) -> json {
	// TODO: Do gamemode flags
	// Ignore player if they are already in queue
	if (player_waiting(uid)) {
		std::cout << "\tPool: Already waiting in queue for a match\n";
		return {};	// Nothing to transmit to frontend
	}

	if (gamemode == "CLASSIC" && players_waiting_classic() < 1) {
		wait_for_classic(uid);
		return {};	// Nothing to transmit to frontend
	}
	else if (gamemode == "TRIPLES" && players_waiting_triples() < 2) {
		wait_for_triples(uid);
		return {};	// Nothing to transmit to frontend
	}
	else {
		return start_player_vs_player_game(gamemode, ranked_flag, uid);
	}
}

auto Pool::start_player_vs_ai_game(std::string const& gamemode, bool ranked_flag, int uid) -> json {
	// Versing AI
	int computer_uid = 6; // TODO: HARDCODED AI ID. Need to do selection
	uint32_t room_id = ((uint32_t) uid << 17) | (uint32_t) computer_uid;
	create_new_room(room_id, ranked_flag, true, {uid, computer_uid});
	return make_json_game_selection({computer_uid, uid}, std::to_string(room_id), gamemode, ranked_flag, true);
}

// === Local Helper functions ===
auto make_json_game_selection(std::vector<int> uids, std::string room_id, std::string gamemode, bool ranked_flag, bool ai_flag) -> json {
	json payload;
	payload["event"] = "match_created";
	payload["uids"] = uids;
	payload["room_id"] = room_id;
	payload["gamemode"] = gamemode;
	payload["elo"] = ranked_flag;
	payload["ai"] = ai_flag;
	std::cout << "\t\tPool: Payload - " << payload.dump() << '\n';
	return payload;
}

auto parse_pool_data(std::string_view const& message) -> json {
	auto json_data = nlohmann::json::parse(message);
	std::string datastring = json_data["data"];
	json data = nlohmann::json::parse(datastring);
	std::cout << "\t\tPool: Data Recieved - "  << data << '\n'; 
	return data;
}