#ifndef SERVER_INVITE_HPP
#define SERVER_INVITE_HPP

#include <nlohmann/json.hpp>

#include "App.h"
#include "room.hpp"
#include "db_manager.hpp"

#include <string>
#include <deque>
#include <algorithm>


using json = nlohmann::json;

struct SocketData{
	//Empty because we don't need any currently.
};

class Invite {
	typedef uWS::WebSocket<false, true, SocketData> *WebSocket;
	private:
		DatabaseManager *db;
		uWS::App *app;
		std::vector<Room *> rooms;
		std::vector<std::pair<int, int>> invites_;

	public:
		Invite(uWS::App *app, DatabaseManager *db)
		: db{db}
		, app{app}
		, rooms{}
		, invites_{}
		{
			app->ws<SocketData>("/ws/inviteroom", uWS::TemplatedApp<false>::WebSocketBehavior<SocketData> {
				.open = [this](auto *ws) {
					std::cout << "\tINVITE: Joined room\n";
					ws->subscribe("DEBUG");
				},
				.message = [this](auto *ws, std::string_view message, uWS::OpCode opCode) mutable {
					// ws->publish("DEBUG", "DEBUG MESSAGE FROM SERVER", opCode);
					std::cout << "\tINVITE: Recieved message\n";
					std::cout << message << '\n';
					json payload = json::parse(message);
					std::cout << "\tINVITE: payload - " << payload << '\n';
					json data = payload["data"];
					std::cout << "\tINVITE: data - " << data << '\n';

					// Player who is sending message
					std::string suid = data["uid"];
					int uid = atoi(suid.c_str());

					// Person has just connected
					if (payload["event"] == "subscribe") {
						std::cout << "\t\tInvite: Just joined and subbed to " << suid << '\n';
						ws->subscribe(suid);
						// Send list of all friends and pending invites
						json message_payload;
						message_payload["event"] = "friends";
						message_payload["friends_uid"];
						message_payload["friends_username"];
						message_payload["pending_uid"];
						message_payload["pending_username"];

						std::vector<User*> friends = this->db->get_friends(uid);
						for (User *user : friends) {
							message_payload["friends_uid"].push_back(user->id);
							message_payload["friends_username"].push_back(user->username);

							// Send any pending invites based on your friends list
							for (std::pair<int,int> const& invite : this->invites_) {
								if (invite.second == uid && invite.first == user->id) {
									message_payload["pending_uid"].push_back(user->id);
									message_payload["pending_username"].push_back(user->username);
									break;
								}
							}
						}
						
						ws->send(message_payload.dump(), opCode);

						return;
					}

					// Person sending invite to their friend
					if (payload["event"] == "send_invite") {
						// Friend selected on frontend --> Send message to this channel
						std::string friend_suid = data["friend"];    
						int friend_uid = atoi(friend_suid.c_str());

																	
						json message_payload;
						message_payload["event"] = "invite";
						message_payload["from_uid"] = uid;
						message_payload["from_username"] = this->db->get_user(uid)->username;

						
						std::cout << "\t\tInvite: Sending invite to friend " << suid;
						ws->publish(friend_suid, message_payload.dump(), opCode);   // Send message to friend
					
						// Store history of invite
						invites_.push_back({uid, friend_uid});
						return;
					}

					// Person accepting invite from their friend
					if (payload["event"] == "accept_invite") {
						// Friend selected on frontend --> Send message to this channel
						std::string friend_suid = data["friend"];    
						int friend_uid = atoi(friend_suid.c_str());

						// Create the room between friends
						uint32_t room_id = ((uint32_t) friend_uid << 16) | (uint32_t) uid;
						create_new_room(room_id, {friend_uid, uid});
						json message_payload = make_json_game_selection({friend_uid, uid}, std::to_string(room_id), "CLASSIC", false, false);

						ws->publish(friend_suid, message_payload.dump(), opCode);       // Send message to friend about room creation
						ws->send(message_payload.dump(), opCode);                       // Send message to person who accepted the invite

						// Remove from invites
						remove_invite(friend_uid, uid);
						return;
					}

					// Person declines invite from their friend
					if (payload["event"] == "decline_invite") {
						// Friend selected on frontend --> Send message to this channel
						std::string friend_suid = data["friend"];    
						int friend_uid = atoi(friend_suid.c_str());

						// Create the room between friends
						json message_payload;
						message_payload["event"] = "declined";
						message_payload["from_uid"] = uid;
						message_payload["from_username"] = this->db->get_user(uid)->username;

						ws->publish(friend_suid, message_payload.dump(), opCode);   // Send message to friend about room creation
						ws->send(message_payload.dump(), opCode);
						
						// Remove from invites
						std::cout << "...Remove: " << friend_uid << " and " << uid << '\n';
						remove_invite(friend_uid, uid);
						return;
					}

				},
				.close = [this](auto *ws, int x , std::string_view str) {
					std::cout << "\tINVITE: Player left waiting room\n";
				}
			});
		}
	private:
		auto create_new_room(uint32_t room_id, std::vector<int> const& uids) -> void {
			replace_room_id(room_id);
			uWS::App &applicaiton = *app;
			rooms.push_back(new Room(applicaiton, db, false, false, false, std::to_string(room_id), uids));
		}
		// Delete old rooms with the same `roomid`
		auto replace_room_id(uint32_t roomid) -> void {
			for (auto &room : rooms) {
				if (room == nullptr) continue;
				if (room->room_id() == std::to_string(roomid)) {
					delete room;
					room = nullptr;  // TODO: Should properly erase from vector instead of setting to null
					break;
				}
			}
		}

		auto remove_invite(int friend_uid, int uid) -> void {
			auto end_it = std::remove_if(
				invites_.begin(),
				invites_.end(),
				[friend_uid, uid](std::pair<int, int> const& data) {
					if(data.first == friend_uid && data.second == uid) return true;
					if(data.second == friend_uid && data.first == uid) return true;
					return false;
				}
			);
			invites_.erase(end_it, invites_.end());
		}
		
		auto make_json_game_selection(std::vector<int> uids, std::string room_id, std::string gamemode, bool ranked_flag, bool ai_flag) -> json {
			json payload;
			payload["event"] = "match_created";
			payload["uids"] = uids;
			payload["room_id"] = room_id;
			payload["gamemode"] = gamemode;
			payload["elo"] = ranked_flag;
			payload["ai"] = ai_flag;
			std::cout << "\tInvite: Payload - " << payload.dump() << '\n';
			return payload;
		}

};

#endif