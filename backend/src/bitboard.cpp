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
	
	for (int i = 0; i < 64; ++i) {
		if (tiles_to_check.isSet(i)) set_tiles.push_back(i);
	}
	return set_tiles;
}


// auto main(void) -> int {
// 	auto const board_width = 9;
// 	auto row = 8;
// 	auto col = 0;
// 	auto res = (row == 0) ? col : ((row * board_width) + col - ((row < 4) ? 10 - (4-row)*(5-row)/2 : (10 + ((row - 4)*(row - 3)/2))));
	
// 	std::cout << res << '\n';
// 	// auto p1 = BitBoard();
// 	// auto p2 = BitBoard();
// 	// // p1.set(1);
// 	// // p1.set(63);
// 	// p1.set(10);
// 	// p2.set(0);
// 	// std::cout << p1.isSet(10) << '\n';
// 	// std::cout << p2.isSet(10) << '\n';

// 	// auto available = ~(p1 ^ p2);
// 	// std::cout << available << '\n';
// 	// std::cout << available.isSet(5) << '\n';

// 	// for (int i = 0; i < 64; ++i) {
// 	// 	std::cout << i << ": " << available.isSet(i) << '\n';
// 	// }


// 	return 0;
// }