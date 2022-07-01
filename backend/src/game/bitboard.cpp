#include <iostream>
#include <bitset>
#include <vector>

#include "bitboard.hpp"

BitBoard::BitBoard()
: board_{(uint64_t) 0}
{}

BitBoard::BitBoard(std::bitset<64> board)
: board_{board.to_ulong()}
{}

// BitBoard::BitBoard(uint64_t state)
// : board_{state}
// {}

auto BitBoard::binary() const -> std::bitset<64> {
	return std::bitset<64>(board_);
}

auto BitBoard::set(int index) -> void {
	board_ = board_ | (uint64_t) 1 << (index);
}

auto BitBoard::unset(int index) -> void {
	board_ = board_ & ~((uint64_t) 1 << (index));
}

auto BitBoard::isSet(int index) -> bool {
	return (board_ & (uint64_t) 1 << (index));
}

auto BitBoard::binary_to_vector() const -> std::vector<int> {
	return binary_to_vector(~BitBoard());
}

auto BitBoard::binary_to_vector(BitBoard const& mask) const -> std::vector<int> {
	auto set_tiles = std::vector<int>{};
	auto tiles_to_check = *this & mask;
	
	for (int i = 0; i < 61; ++i) {
		if (tiles_to_check.isSet(i)) set_tiles.push_back(i);
	}
	return set_tiles;
}
