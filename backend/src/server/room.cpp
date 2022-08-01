#include <nlohmann/json.hpp>
#include <string>
#include <stdio.h>
#include <future>
#include <random>

#include "App.h"
#include "room.hpp"
#include "game.hpp"
#include "aigame.hpp"
#include "metadatagen.hpp"

using json = nlohmann::ordered_json;

// Helper functions
auto parse_move(std::string_view message) -> std::string;
auto parse_uid(std::string_view message) -> int;
auto player_resigned(std::string_view message) -> bool;
auto select_random_player(std::vector<int> choices) -> int;

// auto json_confirm_move(std::string const& move) -> std::string;
// auto json_player3(std::string const& move) -> std::string;
// auto json_board_move(std::string const& move) -> std::string;
// auto json_game_winner(std::string const& player) -> std::string;
// auto game_result(Game const& game) -> std::string;
// auto game_result(int const uid) -> std::string;

auto Room::publish(WebSocket ws, std::string const& message, uWS::OpCode opCode) -> void {
	ws->publish(this->room_id(), message, opCode);
}

auto select_random_player(std::vector<int> choices) -> int {
	std::vector<int> out = {};
	std::sample(
        choices.begin(),
        choices.end(),
        std::back_inserter(out),
        choices.size(),
        std::mt19937{std::random_device{}()}
    );
	return out.front();
}

// ======================================
// 			Class implementation
// ======================================

auto Room::InitRoom(bool ranked, bool computer, bool potholes, int difficulty, std::vector<int> uids) -> void {
	std::cout << "Init room!" << '\n';
	uids_ = uids;
	for (auto uid : uids_) {
		std::cout << uid << ' ';
	}
	std::cout << '\n';

	// What is the gamemode?
	if (uids_.size() == 3) 	gamemode_ = 1;  // Triples
	if (potholes) 			gamemode_ = 2;	// Potholes	

	// Simple flags
	ranked_ = ranked;
	computer_ = computer;
	difficulty_ = difficulty;

	generate_game(potholes);	// CLASSIC / POTHOLES / ETC	
}

Room::Room(uWS::App &app, DatabaseManager *db, bool ranked, bool computer, bool potholes, std::string room_id, std::vector<int> uids, int difficulty)
: room_id_{room_id}
, gamemode_{0} // CLASSIC = 0, TRIPLES = 1, POTHOLES = 2
, uids_{uids}
, game_{nullptr}
, search_{nullptr}
, db_{db}
, ranked_{ranked}
, computer_{computer}
, difficulty_{difficulty}
{
	InitRoom(ranked, computer, potholes, difficulty, uids);
	if (computer_) {
		create_socket_ai(app);
	}
	else { // Versing a human player
		create_socket_player_verse_player(app);
	}
	std::cout << "\t\tGAME FULLY CREATED\n";
}

auto Room::generate_game(bool potholes) -> void {
	std::cout << "\t\tDEBUG: Generating room\n";
	int nplayers = uids_.size();
	std::cout << "NPLAYERS: " << nplayers << '\n';
	BitBoard missing_tiles = BitBoard(); // Assume none
	if (potholes) {
		// Random seed
		auto now = std::chrono::steady_clock::now().time_since_epoch().count();
		auto e1 = std::default_random_engine(now);
		auto d1 = std::uniform_int_distribution<int>(6, 13);
		auto r1 = d1(e1);

		auto potholes = std::vector<int>(61, 0);
		for (int i = 1; i <= r1; i++) {
			auto e2 = std::default_random_engine(now * i);
			auto d2 = std::uniform_int_distribution<int>(0, 60);
			auto r2 = d2(e2);
			if (r2 == 30) continue; // Never get rid of the center tile because BOT3 assumes it's there lol
			potholes.at(r2) = 1;
		}

		auto pots = std::string();
		for (auto const &p : potholes) {
			pots.append(std::to_string(p));
		}
		auto generated_potholes = std::bitset<64>(pots);
		missing_tiles = BitBoard(generated_potholes);
	}
	this->game_ = std::make_unique<Game>(nplayers, uids_, missing_tiles);
	this->search_ = std::make_unique<Search>(nplayers, this->difficulty_, missing_tiles);
	std::cout << "\t\tDEBUG: Generated room\n";
}

auto Room::on_connect_match_info(uWS::WebSocket<false, true, SocketData> *ws) -> void {
	json payload;
	payload["event"] = "player_names";
	payload["player_name"] = {}; // Populate based on current game settings
	payload["elos"] = {};		// Populate based on current game settings

	for (int i = 0; i < this->game_->num_players(); ++i) {
		User *player = this->db_->get_user(this->game_->give_uid(i));
		std::string elo = ((this->ranked_) ? std::to_string(db_->get_latest_elo(player->id, this->gamemode())) : std::string());
		
		payload["player_name"].push_back(player->username);
		payload["elos"].push_back(elo);
	}

	ws->send(payload.dump(), uWS::OpCode::TEXT);

	// Send potholes information if any
	for (auto const& pothole : this->game_->list_potholes()) {
		ws->send(json_pothole(pothole), uWS::OpCode::TEXT);
	}

	if (this->gamemode() == "TRIPLES") return;
	// Difficulty settings
	if (this->game_->num_moves() == 0 && this->difficulty() == 2) {
		this->search_->play(30);
		this->game_->play(30);
		ws->send(json_confirm_move("e5"), uWS::OpCode::TEXT);
	}
	else if (this->difficulty() == 2) {
		auto move = this->game_->all_moves().front();
		ws->send(json_confirm_move("e5"), uWS::OpCode::TEXT);
	}
}

auto Room::create_socket_player_verse_player(uWS::App &app) -> void {
	std::string room_link = this->room_code();
	app.ws<SocketData>(room_link, uWS::TemplatedApp<false>::WebSocketBehavior<SocketData> {
		.open = [this](WebSocket ws) {
			ws->subscribe(this->room_id());
			std::cout << "\tLobby: Joined multiplayer lobby\n";
			on_connect_match_info(ws);
		},
		.message = [this](WebSocket ws, std::string_view message, uWS::OpCode opCode) {
			std::cout << "Lobby: Recieved message\n";
			std::cout << "Lobby: " << message << '\n';
			int uid = parse_uid(message);

			// Pre-game checks
			if (player_resigned(message)) {
				int player = this->game_->give_player_with_uid(uid);
				int winning_player = this->game_->player_after(player);
				std::cout << "\tRoom: Player resigned - winning player: " << winning_player << '\n';

				save_match(winning_player);

				int const winning_uid = this->game_->give_uid(winning_player);
				std::string winner = game_result(winning_uid);
				publish(ws, json_game_winner(winner), opCode);
				return;
			}
			if (click_from_players_turn(uid) == false) return; // Not the players turn
			
			// Player move
			std::string const move = parse_move(message);
			click_register_move(move, ws, opCode);

			// Postgame Checks
			if (game_->status() != Game::state::ONGOING) {
				if (game_->num_players_in() == 3 && game_->status() == Game::state::LOSS) {
					game_->set_player_out(game_->whose_turn());
					
					json payload;
					payload["event"] = "game_over";
					payload["winner"] = "To be decided - You are out!";
					ws->send(payload.dump(), opCode);
					
					game_->continue_game();
					game_->pass_turn();
				}
				else {
					std::string winner = game_result();
					publish(ws, json_game_winner(winner), opCode);
					
					int const winning_player = this->game_->which_player_won();
					save_match(winning_player);
				}
			}
		},
		.close = [this](WebSocket ws, int x , std::string_view str) {
			ws->unsubscribe(this->room_id());
			// ws->close();
			// ws->end();
			std::cout << "Room: Player left lobby\n";
		}
	});
}

// ==== AI ROOM CODE ====

auto Room::create_socket_ai(uWS::App &app) -> void {
	std::string room_link = this->room_code();
	app.ws<SocketData>(room_link, uWS::TemplatedApp<false>::WebSocketBehavior<SocketData> {
		.open = [this](uWS::WebSocket<false, true, SocketData> *ws) {
			ws->subscribe(this->room_id());
			std::cout << "Joined room\n";
			on_connect_match_info(ws);
		},
		.message = [this](WebSocket ws, std::string_view message, uWS::OpCode opCode) {
			std::cout << "Recieved message\n";
			if (player_resigned(message)) {
				std::string winner = game_result(6 + this->difficulty()); // uid 6 is the easiest difficulty
				publish(ws, json_game_winner(winner), opCode);
				save_match(1);
				return;
			}

			int uid = parse_uid(message);
			int players_turn = this->game_->whose_turn();
			int uid_turn = this->game_->give_uid(players_turn);
			if (uid_turn != uid) return; // Not the players turn

			// Get move
			std::string move = parse_move(message);
			
			// Make the move in game
			if (play_move(move) == false) return; 	// Ignore illegal player move
			if (this->gamemode() == "TRIPLES" || this->difficulty() != 2) 	publish(ws, json_confirm_move(move), opCode);
			else { 															publish(ws, json_board_move(move), opCode); }
			// publish(ws, json_player3("a1"), opCode);

			std::cout << "Made a valid move\n";

			if (game_->status() == Game::state::ONGOING) {
				// ONLY FOR TRIPLES: Handle AI moves
				std::cout << "WHAT GAMEMODE? " << this->gamemode() << '\n';
				if (this->gamemode() == "TRIPLES") {	// TODO: Change this to a native call
					Search p1 = Search(*this->game_, select_random_player({2, 2, 0}), this->difficulty());
					int depth = (this->difficulty() == 0) ? 2 : 3;
					int ai1_move = p1.minmax(2);
					this->game_->play(ai1_move);
					publish(ws, json_board_move(Game::indexToCoord(ai1_move)), opCode);

					if (game_->status() == Game::state::ONGOING) {
						Search p2 = Search(*this->game_, select_random_player({1, 0, 0}), this->difficulty());
						if (this->difficulty() == 0) p2 = Search(*this->game_, 0, this->difficulty());
						int depth = (this->difficulty() == 0) ? 2 : 3;
						int ai2_move = p2.minmax(2);
						this->game_->play(ai2_move);
						publish(ws, json_player3(Game::indexToCoord(ai2_move)), opCode);
					}
				}
				// Handle AI moves
				else {
					auto reply = Room::ai_response(move, search_.get(), game_.get(), ws);
					if (this->difficulty() != 2) 	publish(ws, json_board_move(reply), opCode);
					else { 							publish(ws, json_confirm_move(reply), opCode); } // When difficulty 2 is selected
				}
			}


			
			// Postgame
			auto const state = game_->status();
			if (state != Game::state::ONGOING) {
				std::string winner = game_result();
				publish(ws, json_game_winner(winner), opCode);
				int const winning_player = this->game_->which_player_won();
				save_match(winning_player);
			}
			std::cout << "\t\tRoom: finished on message\n";
		},
		.close = [this](WebSocket ws, int x , std::string_view str) {
			ws->unsubscribe(this->room_id());
			// ws->close();
			// ws->end();
			std::cout << "Left ai room\n";
		}
	});
}

auto Room::ai_response(std::string move, Search *aigame, Game *game, void *ws) -> std::string {
	std::cout << "\t\tStarted calculating\n";
	aigame->play(move);
	int depth = 7;  // could increase but would take longer
	auto const response_move = aigame->minmax(depth); 
	aigame->play(response_move);
	game->play(response_move);
	return Game::indexToCoord(response_move);
}

// ======================================
// 			Helper functions
// ======================================
auto player_resigned(std::string_view message) -> bool {
	auto json = nlohmann::json::parse(message);
	return json["event"] == "retire";
}

auto parse_move(std::string_view message) -> std::string {
	auto json = nlohmann::json::parse(message);
	std::string datastring = json["data"];
	auto data = nlohmann::json::parse(datastring);
	return data["move"];
}

auto parse_uid(std::string_view message) -> int {
	auto json = nlohmann::json::parse(message);
	std::cout << json << '\n';
	std::string datastring = json["data"];
	auto data = nlohmann::json::parse(datastring);
	std::string suid = data["uid"];
	return atoi(suid.c_str());
}


// Player 0
auto Room::json_confirm_move(std::string const& move) -> std::string {
	json payload;
	payload["event"] = "moveconfirm";
	payload["tile"] = move;
	payload["turn"] = this->game_->whose_turn();
	return payload.dump();
}

// Player 2
auto Room::json_player3(std::string const& move) -> std::string {
	json payload;
	payload["event"] = "player3";
	payload["tile"] = move;
	payload["turn"] = this->game_->whose_turn();
	return payload.dump();
}

auto Room::json_pothole(std::string const& move) -> std::string {
	json payload;
	payload["event"] = "pothole";
	payload["tile"] = move;
	return payload.dump();
}

// Player 1
auto Room::json_board_move(std::string const& move) -> std::string {
	json payload;
	payload["event"] = "move";
	payload["tile"] = move;
	payload["turn"] = this->game_->whose_turn();
	return payload.dump();
}

auto Room::json_game_winner(std::string const& player) -> std::string {
	json payload;
	payload["event"] = "game_over";
	payload["winner"] = player;
	return payload.dump();
}

auto Room::save_match(int winning_player) -> void {
	// Save Match to Database.
	auto gen = MetaDataGenerator(*(this->game_));
	auto snapshots = gen.db_snapshot();
	// True/False flag for elo
	int const winning_uid = this->game_->give_uid(winning_player);
	// std::cout << "Room: Winning player - " << winning_player << " with uid: " << winning_uid << '\n';
	
	auto playersELO = calc_elos(winning_player);
	// 0 -> Background
	// 1 -> Player 0
	// 2 -> Player 1
	// 3 -> Player 2
	// 4 -> Potholes
	// Initialize all tiles as background initially.
	auto svg = std::vector<int>(61, 0);
	// Update svg_data for players.
	auto i = 1;
	for (auto const &board : game_->board().all_boards()) {
		auto v = board.binary_to_vector();
		for (auto const &t : v) {
			svg.at(t) = i;
		}
		i++;
	}
	// Update svg_data for potholes.
	auto potholes_vec = game_->board().potholes().binary_to_vector();
	for (auto const &t : potholes_vec) {
		svg.at(t) = 4;
	}
	// Create the svg_data string.
	auto svg_data = std::string();
	for (auto const &t : svg) {
		svg_data.append(std::to_string(t));
	}
	// Save Match to DB.
	// TODO: WIN LOSS DRAW MAP in the event of a draw
	auto const match_id = db_->save_match(this->gamemode(), this->ranked_, playersELO, winning_uid,
		game_->list_potholes_string() ,game_->move_sequence(), svg_data, snapshots);
	std::cout << "Match ID: " << match_id << '\n';
}

auto Room::calc_elos(int winning_player) -> std::map<int, int> {
	auto playersELO = std::vector<std::pair<int, int>>{};	// Contains starting elo
	for (auto const &uid : uids_) {
		auto start_elo = db_->get_latest_elo(uid, this->gamemode());
		playersELO.push_back({uid, start_elo});
		std::cout << "\t\tELO: UID: " << uid << " Start elo: " << start_elo << " : " << db_->get_user(uid)->username << '\n';
	}
	// Don't do calculation if there is a draw
	if (this->ranked_ && winning_player != -1) {
		std::cout << "\tRoom: Do elo calculation\n";
		int player = 0;
		for (auto &elo : playersELO) {
			if (player == winning_player) {
				elo.second += 30;
				std::cout << "\t\tELO: UID: " << elo.first << " End elo: " << elo.second << " : " << db_->get_user(elo.first)->username << '\n';
			}
			else {
				elo.second -= 30;
				std::cout << "\t\tELO: UID: " << elo.first << " End elo: " << elo.second << " : " << db_->get_user(elo.first)->username << '\n';
			}
			++player;
		}
	}

	// Put in format for database
	std::map<int, int> final_elos{};
	for (auto const& result : playersELO) {
		final_elos.insert({result.first, result.second});
	}

	return final_elos;
}

// TODO: Make this a game function instead
auto Room::game_result() -> std::string {
	int winning_player = this->game_->which_player_won();
	std::cout << "WINNING PLAYER: " << winning_player << '\n';
	int uid = this->game_->give_uid(winning_player);
	return game_result(uid);
}

auto Room::game_result(int const uid) -> std::string {
	auto user = this->db_->get_user(uid);
	if (!user) return "BOTS";
	return user->username;
}

auto Room::click_register_move(std::string const& move, uWS::WebSocket<false, true, SocketData> *ws, uWS::OpCode opCode) -> void {
	// Make the move in game
	int player_turn = this->game_->whose_turn();				// Need to keep track of current player for message
	if (this->play_move(move) == false) return; 				// Ignore illegal player move
	std::string move_message = publish_move(move, player_turn);	// Send to appropriate player
	publish(ws, move_message, opCode);
}