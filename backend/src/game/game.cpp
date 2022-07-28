#include <string.h>
#include <utility>

#include "game.hpp"
#include "axial.hpp"
#include "bitboard.hpp"

Game::Game(int nplayers, BitBoard potholes) : Game(nplayers, std::vector<int>(nplayers, -1), potholes) {} // When you don't care about uids. Usually for metadata stuff

Game::Game(int nplayers, std::vector<int> const uids, BitBoard potholes)
: BaseGame{nplayers, potholes}
, uids_{uids}
, gamestate_{Game::state::ONGOING}
, move_sequence_{}
{
	// std::cout << "Game: Hello I am making a game for players\n";
	// for (int i : uids) {
	// 	std::cout << "\tUID" << i << '\n';
	// }
}


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
	append_move(BaseGame::indexToCoord(index));
	std::cout << board() << "DEBUG BOARD PRINT\n";
	std::cout << "Game: Spaces - " << board().num_spaces() << " Moves: " << this->num_moves() << "\n";
	return true;
}

auto Game::ongoing() const -> bool {
	return this->status() == Game::state::ONGOING;
}

auto Game::status() const -> Game::state const {
	return gamestate_;
}

auto Game::move_sequence() const -> std::string {
	auto sequence = std::string{};
	for (auto const& move : move_sequence_) {
		sequence.append(move);
	}
	return sequence;
}

auto Game::all_moves() const -> const std::vector<std::string> {
	return move_sequence_;
}

// Private
auto Game::end_turn(int index) -> void {
	this->increase_move();

	// See if the current player won/loss/draw
	if (std::as_const(*this).won(index, whose_turn())) {
		gamestate_ = Game::state::WIN;
		return;
	}
	if (std::as_const(*this).loss(index, whose_turn())) {
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

auto Game::append_move(std::string move) -> void {
	move_sequence_.push_back(move);
}
