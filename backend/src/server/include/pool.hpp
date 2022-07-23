#ifndef SERVER_POOL_HPP
#define SERVER_POOL_HPP

#include "App.h"
#include "room.hpp"
#include "db_manager.hpp"

#include <string>
#include <deque>
#include <algorithm>

class Pool {
	private:
		std::string room_id_;
		std::deque<int> classic_;

		DatabaseManager *db;
		uWS::App *app;
		std::vector<Room *> rooms;
	public:
		Pool(uWS::App *app, DatabaseManager *db);

		auto room_id() const -> std::string;
		auto players_waiting_classic() const -> int {
			return classic_.size();
		}

		auto wait_for_classic(int player_uid) -> void {
			classic_.push_back(player_uid);
		}

		auto player_waiting_in_classic(int player_uid) -> bool {
			auto player = std::find(classic_.begin(), classic_.end(), player_uid);
			return (player != classic_.end());
		}

	private:
		auto create_new_room(uint32_t room_id, bool ranked, bool ai, std::vector<int> const& uids) -> void {
			replace_room_id(room_id);
			uWS::App &applicaiton = *app;
			rooms.push_back(new Room(applicaiton, db, ranked, ai, std::to_string(room_id), uids));
		}

		auto replace_room_id(uint32_t roomid) -> void;	// Delete old rooms with the same `roomid`
		auto create_waiting_room() -> void;				// Create the socketed room

		auto player_vs_player_waiting_lobby(std::string const& gamemode, bool ranked_flag, int uid) -> json;

		auto start_player_vs_player_game(std::string const& gamemode, bool ranked_flag, int uid) -> json;
		auto start_player_vs_ai_game(std::string const& gamemode, bool ranked_flag, int uid) -> json;
};
#endif