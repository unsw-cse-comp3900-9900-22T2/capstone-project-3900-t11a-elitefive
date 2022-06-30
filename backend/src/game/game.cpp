#include <string.h>

#include "game.hpp"
#include "axial.hpp"

Game::Game(int nplayers)
: BaseGame{nplayers}
, uids_{std::vector<int>(nplayers, -1)}
, gamestate_{Game::state::ONGOING}
{}


auto Game::play(std::string move) -> bool {
	if (gamestate_ != Game::state::ONGOING) return false;
	int letter = move[0];
	if (letter >= 'a' && letter <= 'i') {
		auto position = Game::coordToIndex(move);
		if (!board().free_tiles().isSet(position)) return false;
		return play(position);
	}
	std::cout << "Was given: " << move << " ... Failed";
	return false;
}

auto Game::play(int index) -> bool {
	board().set(index, this->whose_turn());
	end_turn(index);
	return true;
}


auto Game::status() -> Game::state {
	return gamestate_;
}


// Private
auto Game::end_turn(int index) -> void {
	this->increase_move();

	// See if the current player won/loss/draw
	if (won(index, whose_turn())) {
		gamestate_ = Game::state::WIN;
		return;
	}
	if (loss(index, whose_turn())) {
		gamestate_ = Game::state::LOSS;
		return;
	}
	if (num_moves() == board().num_spaces()) {
		gamestate_ = Game::state::DRAW;
		return;
	}
	// Game still in play - Update game stats
	pass_turn();
}

auto Game::won(int move, int player) -> bool {
	auto pieces = board().player_tiles(player);
	for (auto const& unit_dir : axial::vector::basis_vectors()){
		auto axis = Board::axis(move, unit_dir);
		if (Board::check_n(pieces, axis, 4)) return true;
	}
	return false;
}

auto Game::loss(int move, int player) -> bool {
	auto pieces = board().player_tiles(player);
	for (auto const& unit_dir : axial::vector::basis_vectors()){
		auto axis = Board::axis(move, unit_dir);
		if (Board::check_n(pieces, axis, 3)) return true;
	}
	return false;
}


