#include <nlohmann/json.hpp>

#include "pool.hpp"
#include "room.hpp"
#include "db_manager.hpp"

#include <string>
#include <queue>
#include <unistd.h>
#include <stdio.h>

using json = nlohmann::ordered_json;

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
			ws->subscribe(this->room_id());
		},
		.message = [this, publish](auto *ws, std::string_view message, uWS::OpCode opCode) mutable {
			
			// Parse data
			json data = parse_pool_data(message);
			std::cout << "\tPool: Message -> " << message << '\n';
			std::string gamemode = data["gamemode"];
			bool ranked_flag = data["ranked"];
			bool ai_flag = data["ai"];

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
				int difficulty = data["ai-difficulty"]; // Either 0, 1, 2
				json payload = start_player_vs_ai_game(gamemode, ranked_flag, uid, difficulty);
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

auto Pool::replace_room_id(uint32_t roomid) -> Room * {
	for (auto &room : rooms) {
		if (room == nullptr) continue;
		if (room->room_id() == std::to_string(roomid)) {
			return room;
		}
	}
	return nullptr;
}

auto Pool::start_player_vs_player_game(std::string const& gamemode, bool ranked_flag, int uid) -> json {
	// We can make a pairing // Player 0: Opponent (First to ready up) // Player 1: Current  (current person to trigger this)
	
	if (gamemode == "CLASSIC") {
		// Grab from Queue
		int opponent = classic_.front();
		classic_.pop_front();

		uint32_t room_id = ((uint32_t) opponent << 16) | (uint32_t) uid;
		create_new_room(room_id, ranked_flag, false, false, {opponent, uid});
		
		json payload = make_json_game_selection({opponent, uid}, std::to_string(room_id), gamemode, ranked_flag, false);
		return payload;
	}
	else if (gamemode == "TRIPLES") {
		int opp1 = triples_.front();
		triples_.pop_front();
		int opp2 = triples_.front();
		triples_.pop_front();

		uint32_t room_id = ((uint32_t) opp1 << 24) | ((uint32_t) opp2 << 16) | uid;
		create_new_room(room_id, ranked_flag, false, false, {opp1, opp2, uid});
		json payload = make_json_game_selection({opp1, opp2, uid}, std::to_string(room_id), gamemode, ranked_flag, false);
		return payload;
	}
	else if (gamemode == "POTHOLES") {
		// Grab from Queue
		int opponent = potholes_.front();
		potholes_.pop_front();

		uint32_t room_id = ((uint32_t) opponent << 16) | (uint32_t) uid;
		// ai: false
		// potholes: true 
		create_new_room(room_id, ranked_flag, false, true, {opponent, uid});
		
		json payload = make_json_game_selection({opponent, uid}, std::to_string(room_id), gamemode, ranked_flag, false);
		return payload;
	}
}

auto Pool::player_vs_player_waiting_lobby(std::string const& gamemode, bool ranked_flag, int uid) -> json {
	// Ignore player if they are already in queue
	if (player_waiting(uid, gamemode)) {
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
	else if (gamemode == "POTHOLES" && players_waiting_potholes() < 1) {
		wait_for_potholes(uid);
		return {};
	}
	else {
		return start_player_vs_player_game(gamemode, ranked_flag, uid);
	}
}

auto Pool::start_player_vs_ai_game(std::string const& gamemode, bool ranked_flag, int uid, int difficulty) -> json {
	// Versing AI
	int computer_uid = 6 + difficulty; 
	std::vector<int> uids = {uid, computer_uid};
	if (gamemode != "TRIPLES" && difficulty == 2) {
		uids = {computer_uid, uid};
	}

	std::cout << "STARTING A NEW GAME " << gamemode << "\n";

	
	uint32_t room_id = ((uint32_t) uid << 17) | (uint32_t) computer_uid;
	if (gamemode == "TRIPLES") {
		uids.push_back(8);
		room_id += 8;
	}
	
	bool potholes = false;
	if (gamemode == "POTHOLES") potholes = true;
	create_new_room(room_id, ranked_flag, true, potholes, uids, difficulty);
	return make_json_game_selection(uids, std::to_string(room_id), gamemode, ranked_flag, true);
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