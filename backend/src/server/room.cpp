#include <nlohmann/json.hpp>
#include <string>

#include "App.h"
#include "room.hpp"
#include "game.hpp"
#include "aigame.hpp"
#include "metadatagen.hpp"

struct SocketData{
	//Empty because we don't need any currently.
};

// Helper functions
auto parse_move(std::string_view message) -> std::string;
auto json_confirm_move(std::string const& move) -> std::string;
auto json_board_move(std::string const& move) -> std::string;
auto json_game_winner(std::string const& player) -> std::string;
auto game_result(Game const& game) -> std::string;

// ======================================
// 			Class implementation
// ======================================
Room::Room(uWS::App &app, DatabaseManager *db, std::string room_id, std::vector<int> uids)
: room_id_{room_id}
, uids_{uids}
, game_{nullptr}
, aigame_{nullptr}
, db_{db}
{
	generate_game();	// CLASSIC / POTHOLES / ETC
	// create_socket_ai(app);
	if (game_ == nullptr) exit(1);
	create_socket_player_verse_player(app);
	std::cout << "MATCH CREATED: Lobby is setup\n";
}

auto Room::generate_game() -> void {
	int nplayers = uids_.size();
	std::cout << "Generating game: " << nplayers << '\n';
	for (auto const &i : uids_) {
		std::cout << i << '\n';
	}

	this->game_ = std::make_unique<Game>(nplayers, uids_);
	this->aigame_ = std::make_unique<AIGame>(nplayers);
}

auto Room::create_socket_player_verse_player(uWS::App &app) -> void {
	auto publish = [this](auto *ws, std::string message, uWS::OpCode opCode) -> void {
		std::cout << "\tRoom: Publishing to - " << this->room_id() << '\n';
		ws->publish(this->room_id(), message, opCode);
	};

	std::string room_link = this->room_code();
	std::cout << "ROOM: Player verse player room being created\n";
	app.ws<SocketData>(room_link, uWS::TemplatedApp<false>::WebSocketBehavior<SocketData> {
		.open = [this, publish](auto *ws) {
			ws->subscribe(this->room_id());
			std::cout << "Lobby: Joined multiplayer lobby\n";
		},
		.message = [this, publish](auto *ws, std::string_view message, uWS::OpCode opCode) {
			std::cout << "Lobby: Recieved message\n";
			// Get move
			std::string move = parse_move(message);
			std::cout << "Lobby: Finished Parsing\n";
			std::cout << "Lobby: " << move << '\n';

			// Make the move in game
			std::string move_message = publish_move(move);
			std::cout << "Lobby: Playing Game move\n";
			if (this->play_move(move) == false) return; 	// Ignore illegal player move
			std::cout << "Lobby: Publishing move\n";
			publish(ws, move_message, opCode);
			// publish(ws, json_confirm_move(move), opCode);
			// publish(ws, json_board_move(move), opCode);
			std::cout << "Lobby: Made a valid move\n";
		
			// Postgame
			auto const state = game_->status();
			if (state != Game::state::ONGOING) {
				std::string winner = game_result(*this->game_);
				publish(ws, json_game_winner(winner), opCode);
				// Save Match to Database.
				// TODO: This needs to be modified.
				auto playersELO = std::map<int, int>{};
				for (auto const &uid : uids_) {
					playersELO.insert({uid, 0});
				}
				std::cout << "Room: Postgame metadata creation\n"; 
				auto gen = MetaDataGenerator(*game_);
				std::cout << "Room: Postgame metadata created\n"; 
				auto snapshots = gen.db_snapshot();
				std::cout << "Room: Save the match in db\n"; 
				if (db_ == nullptr) {
					std::cout << "Room: Dabase is null\n";
				}
				// ws->end();
				// auto const match_id = db_->save_match("CLASSIC", false, playersELO, -1, game_->move_sequence(), snapshots);
				// std::cout << "Match ID: " << match_id << '\n';
			}
			// auto const match_id = db.save_match("CLASSIC", game->move_sequence());
		},
		.close = [this](auto *ws, int x , std::string_view str) {
			ws->unsubscribe(this->room_id());
			// ws->close();
			// ws->end();
			std::cout << "Player left lobby\n";
		}
	});
}

auto Room::create_socket_ai(uWS::App &app) -> void {
	auto publish = [this](auto *ws, std::string message, uWS::OpCode opCode) -> void {
		ws->publish(this->room_id(), message, opCode);
	};

	std::string room_link = this->room_code();
	std::cout << "Room code: " << room_link << '\n';
	std::string temp_link = "/ws/david";

	app.ws<SocketData>(temp_link, uWS::TemplatedApp<false>::WebSocketBehavior<SocketData> {
		.open = [this, publish](auto *ws) {
			ws->subscribe(this->room_id());
			std::cout << "Joined room\n";
		},
		.message = [this, publish](auto *ws, std::string_view message, uWS::OpCode opCode) {
			std::cout << "Recieved message\n";
			// Get move
			std::string move = parse_move(message);
			
			// Make the move in game
			if (play_move(move) == false) return; 	// Ignore illegal player move
			publish(ws, json_confirm_move(move), opCode);
			std::cout << "Made a valid move\n";

			// Handle AI moves
			if (game_->status() == Game::state::ONGOING) {	// TODO: Change this to a native call
				std::string reply = ai_response(move);
				publish(ws, json_board_move(reply), opCode);
				// gameover = (game_->status() == Game::state::ONGOING);
			}
			
			// Postgame
			auto const state = game_->status();
			if (state != Game::state::ONGOING) {
				std::string winner = game_result(*this->game_);
				publish(ws, json_game_winner(winner), opCode);
				// Save Match to Database.
				// TODO: This needs to be modified.
				auto playersELO = std::map<int, int>{};
				for (auto const &uid : uids_) {
					// TODO: Give the new elo calculation
					playersELO.insert({uid, 0});
				}
				auto gen = MetaDataGenerator(*game_);
				auto snapshots = gen.db_snapshot();
				// True/False flag for elo
				int const winning_player = this->game_->which_player_won();
				int const winning_uid = this->game_->give_uid(winning_player);
				std::cout << "Room: Winning player - " << winning_player << " with uid: " << winning_uid << '\n';
				auto const match_id = db_->save_match("CLASSIC", false, playersELO, winning_uid,
					game_->move_sequence(), snapshots);
				std::cout << "Match ID: " << match_id << '\n';
			}
			// auto const match_id = db.save_match("CLASSIC", game->move_sequence());
		},
		.close = [this](auto *ws, int x , std::string_view str) {
			ws->unsubscribe(this->room_id());
			ws->close();
			std::cout << "Left ai room\n";
		}
	});
}

auto Room::room_id() const -> std::string {
	return room_id_;
}

auto Room::room_code() const -> std::string {
	return std::string{"/ws/game/" + this->room_id()};
}

auto Room::play_move(std::string const& move) -> bool {
	return this->game_->play(move);
}

auto Room::ai_response(std::string const& move) -> std::string {
	AIGame &aigame = *aigame_;	
	aigame.play(move);
	auto const response_move = aigame.minmax(3); // depth 3 (could increase but test with that)
	aigame.play(response_move);
	aigame.clear();
	
	// Reflect move in the game
	game_->play(response_move);
	return Game::indexToCoord(response_move);
}

// ======================================
// 			Helper functions
// ======================================
auto parse_move(std::string_view message) -> std::string {
	auto json = nlohmann::json::parse(message);
	std::string datastring = json["data"];
	auto data = nlohmann::json::parse(datastring);
	return data["move"];
}

auto Room::json_confirm_move(std::string const& move) -> std::string {
	return "{\"event\": \"moveconfirm\", \"tile\": \"" + move + "\"}";
}

auto Room::json_board_move(std::string const& move) -> std::string {
	return "{\"event\": \"move\", \"tile\": \"" + move + "\"}";
}

auto Room::json_game_winner(std::string const& player) -> std::string {
	return "{\"event\": \"game_over\", \"winner\": \"" + player + "\"}";
}

// TODO: Make this a game function instead
auto game_result(Game const& game) -> std::string {
	// auto const state = game.status();
	// std::string winner;
	// if (state == Game::state::DRAW) {
	// 	winner = "";
	// } else if (game.whose_turn() == 1 && state == Game::state::WIN
	// 		|| game.whose_turn() == 0 && state == Game::state::LOSS) {
	// 	winner = "COMPUTER";
	// } else {
	// 	winner = "PLAYER";
	// }
	// return winner;
	int winning_player = game.which_player_won();
	if (winning_player == 0) return "PLAYER";
	if (winning_player == 1) return "COMPUTER";
	return "MISSINGNO";
}

