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
		std::deque<int> triples_;
		std::deque<int> potholes_;

		DatabaseManager *db;
		uWS::App *app;
		std::vector<Room *> rooms;
	public:
		Pool(uWS::App *app, DatabaseManager *db);

		auto room_id() const -> std::string { return room_id_; }

		auto players_waiting_classic() const -> int { return classic_.size(); }
		auto players_waiting_triples() const -> int { return triples_.size(); }
		auto players_waiting_potholes() const -> int { return potholes_.size(); }

		auto wait_for_classic(int player_uid) -> void { classic_.push_back(player_uid); }
		auto wait_for_triples(int player_uid) -> void { triples_.push_back(player_uid); }
		auto wait_for_potholes(int player_uid) -> void { potholes_.push_back(player_uid); }

		// Check the queue
		auto player_waiting(int uid, std::string const& gamemode) -> bool {
			auto remove = [uid](std::deque<int> &queue) {
				auto end_it = std::remove_if(
					queue.begin(),
					queue.end(),
					[uid](int const& data) {
						return (data == uid);
					}
				);
				queue.erase(end_it, queue.end());
			};
			
			if (player_waiting_in_classic(uid)) {
				if (gamemode == "CLASSIC") return true;
				remove(classic_);
				return false;
			}
			if (player_waiting_in_triples(uid)) {
				if (gamemode == "TRIPLES") return true;
				remove(triples_);
				return false;

			}
			if (player_waiting_in_potholes(uid)) {
				if (gamemode == "POTHOLES") return true;
				remove(potholes_);
				return false;
			}
			return false;
		}

		auto player_waiting_in_classic(int player_uid) -> bool {
			auto player = std::find(classic_.begin(), classic_.end(), player_uid);
			return (player != classic_.end());
		}
		auto player_waiting_in_triples(int player_uid) -> bool {
			auto player = std::find(triples_.begin(), triples_.end(), player_uid);
			return (player != triples_.end());
		}		
		auto player_waiting_in_potholes(int player_uid) -> bool {
			auto player = std::find(potholes_.begin(), potholes_.end(), player_uid);
			return (player != potholes_.end());
		}

	private:
		auto create_new_room(uint32_t room_id, bool ranked, bool ai, bool potholes, std::vector<int> const& uids) -> void {
			std::cout << "\t\tDEBUG: REPLACE ROOM\n";
			replace_room_id(room_id);
			std::cout << "\t\tDEBUG: REPLACED\n";
			uWS::App &applicaiton = *app;
			std::cout << "\t\tDEBUG: MAKE NEW ROOM\n";
			rooms.push_back(new Room(applicaiton, db, ranked, ai, potholes, std::to_string(room_id), uids));
			std::cout << "\t\tDEBUG: MADE NEW ROOM\n";

		}

		auto replace_room_id(uint32_t roomid) -> void;	// Delete old rooms with the same `roomid`

		auto player_vs_player_waiting_lobby(std::string const& gamemode, bool ranked_flag, int uid) -> json;

		auto start_player_vs_player_game(std::string const& gamemode, bool ranked_flag, int uid) -> json;
		auto start_player_vs_ai_game(std::string const& gamemode, bool ranked_flag, int uid) -> json;
};
#endif