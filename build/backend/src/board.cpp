#include <vector>

#include "board.hpp"
#include "bitboard.hpp"


Board::Board(int nplayers)
: nplayers_{nplayers}
, player_boards_{std::vector<BitBoard>{}}
{
	for (int i = 0; i < nplayers; i++) {
		player_boards_.push_back(BitBoard()); // Every player has an empty board
	}
}

auto Board::set(int location, int player) -> void {
	player_boards_[player].set(location);
}

auto Board::unset(int location, int player) -> void {
	player_boards_[player].unset(location);
}

auto Board::which_player_at(int location) const -> int {
	for (int player = 0; player < nplayers_; player++) {
		auto board = player_tiles(player);
		if (board.isSet(location)) return player;
	}
	return -1;
}

auto Board::player_tiles(int player) const -> BitBoard {
	return player_boards_[player];
}

auto Board::free_tiles() -> BitBoard {
	auto taken_spaces = BitBoard();
	for (auto const& player : player_boards_) {
		taken_spaces = taken_spaces ^ player;
	}
	return ~taken_spaces;	// Negate to get free spaces
}


auto Board::free_tiles(int tile, axial::vector unit_direction) -> BitBoard {
	return free_tiles(axial::vector(tile), unit_direction);
}

auto Board::free_tiles(axial::vector anchor_point, axial::vector unit_direction) -> BitBoard {
	auto candidate_tiles = Board::axis(anchor_point, unit_direction);
	auto free_spaces = free_tiles();

	return candidate_tiles & free_spaces;
}

auto Board::num_players() -> int {
	return nplayers_;
}


// Static functions
auto Board::axis(int tile, axial::vector unit_direction) -> BitBoard {
	return axis(axial::vector(tile), unit_direction);
}

auto Board::axis(axial::vector anchor_point, axial::vector unit_direction) -> BitBoard {
	auto spaces = BitBoard();
	// Go along axis in unit vector direction
	axial::vector pos = anchor_point;
	while (pos.distance() <= 4) {
		spaces.set(axial::vector::index(pos));
		pos += unit_direction;
	}
	
	// Go along axis in other direction
	pos = anchor_point - unit_direction;
	while (pos.distance() <= 4) {
		spaces.set(axial::vector::index(pos));
		pos -= unit_direction;
	}

	return spaces;
}

auto Board::check_n(BitBoard tiles, BitBoard diagonal, int n) -> bool {
	auto const placed = tiles.binary_to_vector(diagonal);
	auto const check_along = diagonal.binary_to_vector();

	int count = 0;
	auto curr = placed.begin();
	for (auto const& i : check_along) {
		if (curr == placed.end()) break;
		if (i == *curr) {
			++count;
			++curr;
		}
		else {
			count = 0;
		}
		if (count == n) return true;
	}
	return false;
}
