#include <assert.h> // TODO: REMOVE
#include "basegame.hpp"

BaseGame::BaseGame(int players)
: board_{Board(players)}
, player_turn_{0}
, nmoves_{0}
{}

BaseGame::BaseGame(Board const& board, int turn, int moves)
: board_{Board(board)}
, player_turn_{turn}
, nmoves_{moves}
{}

auto BaseGame::whose_turn() const -> int {
	return player_turn_;
}
auto BaseGame::num_moves() const -> int {
	return nmoves_;
}

auto BaseGame::board() -> Board & {
	return board_;
}

auto BaseGame::board() const -> Board const& {
	return board_;
}

auto BaseGame::increase_move() -> void {
	++nmoves_;
}

auto BaseGame::next_player() const -> int {
	return (player_turn_ + 1) % board().num_players();
}

auto BaseGame::player_after(int curr_player) const -> int {
	return (curr_player + 1) % board().num_players();
}

auto BaseGame::previous_player() const -> int {
	return (player_turn_ == 0) ? board().num_players() - 1 : player_turn_ - 1;
}

auto BaseGame::num_players() const -> int {
	return this->board_.num_players();
}


auto BaseGame::pass_turn() -> void {
	player_turn_ = next_player();
}

auto BaseGame::decrease_move() -> void {
	--nmoves_;
}

auto BaseGame::unpass_turn() -> void {
	player_turn_ = previous_player();
	assert(player_turn_ >= 0); // TODO: REMOVE
}


// =================
// Static functions
// =================
auto BaseGame::indexToCoord(int index) -> const std::string {
	int const q = calculate_q(index);
	char const letter = 'a' + q + 4;
	int const column = index - floor_index(q) + 1;
	return letter + std::to_string(column);
}

auto BaseGame::coordToIndex(std::string coord) -> int {
	int const letter = coord[0];
	if (letter >= 'a' && letter <= 'i') {
		return floor_index(letter - 'a' - 4) + stoi(coord.substr(1)) - 1;
	}
	return -1;
}

auto BaseGame::floor_index(int q) -> int {
	return ((q + 8) * (q + 9) / 2) - 10 - ((q > 1) ? q * (q - 1) : 0);
}

int BaseGame::calculate_q(int index) {
	for (int q = -4; q < 5; ++q) {
		if (index < floor_index(q + 1)) {
			return q;
		}
	}
	return -1;
}

// Private functions
auto BaseGame::won(int move, int player) const -> bool {
	auto const& pieces = board().player_tiles(player);
	for (auto const& unit_dir : axial::vector::basis_vectors()) {
		auto const axis = Board::axis(move, unit_dir);
		if (Board::check_n(pieces, axis, 4)) return true;
	}
	return false;
}

auto BaseGame::loss(int move, int player) const -> bool {
	auto const& pieces = board().player_tiles(player); 
	for (auto const& unit_dir : axial::vector::basis_vectors()) {
		auto const axis = Board::axis(move, unit_dir);
		if (Board::check_n(pieces, axis, 3)) return true;
	}
	return false;
}
