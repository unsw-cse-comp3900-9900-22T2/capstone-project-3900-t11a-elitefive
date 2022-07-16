#include <nlohmann/json.hpp>
#include <string>

#include "App.h"
#include "room.hpp"
#include "game.hpp"
#include "aigame.hpp"

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
Room::Room(uWS::App &app, DatabaseManager *db, std::vector<int> uids)
: room_id_{"testing"}
, uids_{uids}
, game_{nullptr}
, aigame_{nullptr}
, db_{db}
{
	generate_game();	// CLASSIC / POTHOLES / ETC
	create_socket_ai(app);
	std::cout << "Room is setup\n";
}

auto Room::generate_game() -> void {
	int nplayers = uids_.size();
	this->game_ = std::make_unique<Game>(nplayers);
	this->aigame_ = std::make_unique<AIGame>(nplayers);
}

auto Room::create_socket_ai(uWS::App &app) -> void {
	auto publish = [this](auto *ws, std::string message, uWS::OpCode opCode) -> void {
		ws->publish(this->room_id(), message, opCode);
	};

	app.ws<SocketData>("/ws/david",uWS::TemplatedApp<false>::WebSocketBehavior<SocketData> {
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
				// auto const match_id = db_->save_match("CLASSIC", game_->move_sequence());
				// std::cout << "Match ID: " << match_id << '\n';
			}
			// auto const match_id = db.save_match("CLASSIC", game->move_sequence());
		},
		.close = [this](auto *ws, int x , std::string_view str) {
			ws->unsubscribe(this->room_id());
			ws->close();
			std::cout << "Room is destroyed\n";
		}
	});
}

auto Room::room_id() const -> std::string {
	return room_id_;
}

auto Room::play_move(std::string const& move) -> bool {
	return game_->play(move);
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

auto json_confirm_move(std::string const& move) -> std::string {
	return "{\"event\": \"moveconfirm\", \"tile\": \"" + move + "\"}";
}

auto json_board_move(std::string const& move) -> std::string {
	return "{\"event\": \"move\", \"tile\": \"" + move + "\"}";
}

auto json_game_winner(std::string const& player) -> std::string {
	return "{\"event\": \"game_over\", \"winner\": \"" + player + "\"}";
}

// TODO: Make this a game function instead
auto game_result(Game const& game) -> std::string {
	auto const state = game.status();
	std::string winner;
	if (state == Game::state::DRAW) {
		winner = "";
	} else if (game.whose_turn() == 1 && state == Game::state::WIN
			|| game.whose_turn() == 0 && state == Game::state::LOSS) {
		winner = "COMPUTER";
	} else {
		winner = "PLAYER";
	}
	return winner;
}

