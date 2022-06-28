#include <iostream>
#include <bitset>

#include "bitboard.hpp"

BitBoard::BitBoard()
: board_{(uint64_t) 0}
{}

BitBoard::BitBoard(std::bitset<64> board)
: board_{board.to_ulong()}
{}

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

auto main(void) -> int {
	auto p1 = BitBoard();
	auto p2 = BitBoard();
	// p1.set(1);
	// p1.set(63);
	p1.set(10);
	p2.set(0);
	std::cout << p1.isSet(10) << '\n';
	std::cout << p2.isSet(10) << '\n';

	auto available = ~(p1 ^ p2);
	std::cout << available << '\n';
	std::cout << available.isSet(5) << '\n';

	for (int i = 0; i < 64; ++i) {
		std::cout << i << ": " << available.isSet(i) << '\n';
	}


	return 0;
}