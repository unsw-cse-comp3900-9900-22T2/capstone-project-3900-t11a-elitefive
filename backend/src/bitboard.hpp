#ifndef YAVALATH_BOARD_HPP
#define YAVALATH_BOARD_HPP

#include <iostream>
#include <bitset>

class BitBoard {
	private:
		uint64_t board_;
	public:
		BitBoard();
		BitBoard(std::bitset<64> board);

		auto binary() const -> std::bitset<64>;		// Gives binary representation
		auto set(int index) -> void;				// Set bit  : Index right to left
		auto unset(int index) -> void;				// Unset bit: Index right to left

		auto isSet(int index) -> bool;

		// Operators
		friend auto operator<<(std::ostream& os, BitBoard const& bitboard) -> std::ostream& {
			os << bitboard.binary();
			return os;
		}

		friend auto operator^(BitBoard const& b1, BitBoard const& b2) -> BitBoard {
			return BitBoard(b1.binary() ^ b2.binary());
		}

		friend auto operator~(BitBoard const& b1) -> BitBoard {
			return BitBoard(~b1.binary());
		}
};

#endif