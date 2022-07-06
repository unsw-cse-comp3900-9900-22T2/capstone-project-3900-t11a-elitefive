#ifndef YAVALATH_BITBOARD_HPP
#define YAVALATH_BITBOARD_HPP

#include <iostream>
#include <bitset>
#include <vector>

class BitBoard {
	private:
		uint64_t board_;
	public:
		BitBoard();
		BitBoard(std::bitset<64> const& board);
		// BitBoard(uint64_t state);

		auto value() const -> uint64_t;				// Gives the uint64_t representation
		auto binary() const -> std::bitset<64>;		// Gives binary representation
		auto set(int index) -> void;				// Set bit  : Index right to left
		auto unset(int index) -> void;				// Unset bit: Index right to left
		auto isSet(int index) const -> bool;

		auto binary_to_vector() const -> std::vector<int>;
		auto binary_to_vector(BitBoard const& mask) const -> std::vector<int>;


		// Operators
		friend auto operator<<(std::ostream& os, BitBoard const& bitboard) -> std::ostream& {
			os << bitboard.binary();
			return os;
		}

		friend auto operator==(BitBoard const& b1, BitBoard const& b2) -> bool {
			return (b1.value() == b2.value());
		}

		friend auto operator|(BitBoard const& b1, BitBoard const& b2) -> BitBoard {
			return BitBoard(b1.binary() | b2.binary());
		}

		friend auto operator^(BitBoard const& b1, BitBoard const& b2) -> BitBoard {
			return BitBoard(b1.binary() ^ b2.binary());
		}

		friend auto operator&(BitBoard const& b1, BitBoard const& b2) -> BitBoard {
			return BitBoard(b1.binary() & b2.binary());
		}	

		friend auto operator~(BitBoard const& b1) -> BitBoard {
			return BitBoard(~b1.binary());
		}
};

#endif