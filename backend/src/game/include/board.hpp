#ifndef YAVALATH_BOARD_HPP
#define YAVALATH_BOARD_HPP

#include <vector>
#include <string>

#include "bitboard.hpp"
#include "axial.hpp"

class Board {
	public:
		auto static axis(int tile, axial::vector unit_direction) -> BitBoard;
		auto static axis(axial::vector anchor_point, axial::vector unit_direction) -> BitBoard;
		auto static check_n(BitBoard tiles, BitBoard diagonal, int n) -> bool;

	private:
		int nplayers_;
		int nspaces_;
		std::vector<BitBoard> player_boards_;

	public:
		Board(int nplayers);
		Board(Board const& board);
		
		auto set(int location, int player) -> void;
		auto unset(int location, int player) -> void;

		auto player_at(int location) const -> int;
		auto player_tiles(int player) const -> BitBoard;
		auto free_tiles() -> BitBoard;
		auto free_tiles(int tile, axial::vector unit_direction) -> BitBoard;
		auto free_tiles(axial::vector anchor_point, axial::vector unit_direction) -> BitBoard;

		auto num_players() const -> int;
		auto num_spaces() const -> int;

		friend auto operator<<(std::ostream& os, Board const& board) -> std::ostream& {
			auto const board_width = 9;	// Static board size assumption

			auto tile_lookup = [board, board_width](int row, int col, int triangle_offset) {
				auto position = row * 9 + col - triangle_offset;	// Where 9 is the board with
				auto player = board.player_at(position);
				if (player == 0) return 'o';
				if (player == 1) return 'x';
				return '-';
			};


			auto triangle_offset = 0;
			for (int row = -4; row <= 4 ; ++row) {
				auto const spacing = (row < 0) ? -row : row;
				for (int i = 0; i < spacing; ++i) {
					os << " ";
				}
				for (int i = 0; i < board_width - spacing; ++i) {
					auto const col = (row < 0) ? 0 : row;
					auto symbol = tile_lookup(row + 4, col + i, triangle_offset);
					os << symbol << " ";
				}
				os << '\n';
				triangle_offset += (row < 0) ? spacing : row + 1;
			}

			return os;
		}
};


#endif