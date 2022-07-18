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
	public:
		Pool(uWS::App &app, DatabaseManager *db, Room *room);

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
		auto create_waiting_room(uWS::App &app, DatabaseManager *db, Room *room) -> void;
};
#endif