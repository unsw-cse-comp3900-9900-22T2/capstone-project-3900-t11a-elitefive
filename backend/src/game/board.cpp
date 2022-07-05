#include <vector>

#include "board.hpp"
#include "bitboard.hpp"


Board::Board(int nplayers)
: nplayers_{nplayers}
, nspaces_{61} // Assume that it is a classic board
, player_boards_{std::vector<BitBoard>{}}
{
	for (int i = 0; i < nplayers; i++) {
		player_boards_.push_back(BitBoard()); // Every player has an empty board
	}
}

Board::Board(Board const& board)
: nplayers_{board.num_players()}
, nspaces_{board.num_spaces()}
, player_boards_{std::vector<BitBoard>{}}
{
	for (int i = 0; i < nplayers_; i++) {
		player_boards_.push_back(board.player_tiles(i)); // Every player current board
	}
}

// MODIFYING ACTIONS
auto Board::set(int location, int player) -> void {
	player_boards_[player].set(location);
}

auto Board::unset(int location, int player) -> void {
	player_boards_[player].unset(location);
}


// CONST QUERY ACTIONS
auto Board::player_at_location(int player, int location) const -> bool {
	auto const& board = player_tiles(player);
	return board.isSet(location);
}

auto Board::which_player_at(int location) const -> int {
	for (int player = 0; player < nplayers_; player++) {
		auto const &board = player_tiles(player);
		if (board.isSet(location)) return player;
	}

	return -1;
}

auto Board::num_players() const -> int {
	return nplayers_;
}

auto Board::num_spaces() const -> int {
	return nspaces_;
}

auto Board::player_tiles(int player) const -> BitBoard {
	return player_boards_[player];
}

auto Board::opponent_tiles(int player) const -> BitBoard {
	auto const& player_tiles = this->player_tiles(player);
	auto const& free_tiles = this->free_tiles();
	return (~free_tiles) ^ player_tiles;
}

auto Board::free_tiles() const -> BitBoard {
	auto taken_spaces = BitBoard();
	for (auto const& player : player_boards_) {
		taken_spaces = taken_spaces ^ player;
	}
	return ~taken_spaces;	// Negate to get free spaces
}


auto Board::free_tiles(int tile, axial::vector const& unit_direction) const -> BitBoard {
	return free_tiles(axial::vector(tile), unit_direction);
}

auto Board::free_tiles(axial::vector const& anchor_point, axial::vector const& unit_direction) const -> BitBoard {
	auto const candidate_tiles = Board::axis(anchor_point, unit_direction);
	auto const free_spaces = free_tiles();

	return candidate_tiles & free_spaces;
}

auto Board::all_boards() const -> std::vector<BitBoard> {
	return player_boards_;
}



// Static functions
auto Board::axis(int tile, axial::vector const& unit_direction) -> BitBoard {
	return axis(axial::vector(tile), unit_direction);
}

auto Board::axis(axial::vector const& anchor_point, axial::vector const& unit_direction) -> BitBoard {
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

auto Board::check_n(BitBoard const& tiles, BitBoard const& diagonal, int n) -> bool {
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
