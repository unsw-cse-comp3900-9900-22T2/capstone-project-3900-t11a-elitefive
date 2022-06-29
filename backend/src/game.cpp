#include <string.h>

#include "game.hpp"
#include "axial.hpp"

auto end_turn(int index) -> void;
auto is_win(int move, int player) -> bool;

Game::Game(int nplayers)
: Board{nplayers}
, player_turn_{0}
, nmoves_{0}
, total_spaces_{61} // Could change for potholes
, uids_{std::vector<int>(nplayers, -1)}
, gamestate_{Game::state::ONGOING}
{}


auto Game::play(std::string move) -> bool{
	if (gamestate_ != Game::state::ONGOING) return false;
	int letter = move[0];
	if (letter >= 'a' && letter <= 'i') {
		auto position = Game::coordToIndex(move);
		if (!free_tiles().isSet(position)) return false;
		return play(position);
	}
	std::cout << "Was given: " << move << " ... Failed";
	return false;
}

auto Game::play(int index) -> bool {
	set(index, player_turn_);
	end_turn(index);
	return true;
}


auto Game::num_moves() -> int {
	return nmoves_;
}

auto Game::status() -> Game::state {
	return gamestate_;
}

auto Game::whose_turn() -> int {
	return player_turn_;
}


// Private
auto Game::end_turn(int index) -> void {
	++nmoves_;

	// See if the current player won/loss/draw
	if (won(index, whose_turn())) {
		gamestate_ = Game::state::WIN;
		return;
	}
	if (loss(index, whose_turn())) {
		gamestate_ = Game::state::LOSS;
		return;
	}
	if (num_moves() == total_spaces_) {
		gamestate_ = Game::state::DRAW;
		return;
	}
	// Game still in play - Update game stats
	player_turn_ = (player_turn_ + 1) % num_players();
}

auto Game::won(int move, int player) -> bool {
	auto pieces = player_tiles(player);
	for (auto const& unit_dir : axial::vector::basis_vectors()){
		auto axis = Board::axis(move, unit_dir);
		if (Board::check_n(pieces, axis, 4)) return true;
	}
	return false;
}

auto Game::loss(int move, int player) -> bool {
	auto pieces = player_tiles(player);
	for (auto const& unit_dir : axial::vector::basis_vectors()){
		auto axis = Board::axis(move, unit_dir);
		if (Board::check_n(pieces, axis, 3)) return true;
	}
	return false;
}


