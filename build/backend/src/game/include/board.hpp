#ifndef YAVALATH_BOARD_HPP
#define YAVALATH_BOARD_HPP

#include <vector>
#include <string>

#include "bitboard.hpp"
#include "axial.hpp"

class Board {
	public:
		auto static axis(int tile, axial::vector const& unit_direction) -> BitBoard;
		auto static axis(axial::vector const& anchor_point, axial::vector const& unit_direction) -> BitBoard;
		auto static check_n(BitBoard const& tiles, BitBoard const& diagonal, int n) -> bool;

	private:
		int nplayers_;
		int nspaces_;
		std::vector<BitBoard> player_boards_;
		BitBoard potholes_;

	public:
		Board(int nplayers);
		Board(int nplayers, BitBoard const& potholes);
		Board(Board const& board);
		
		// MODIFYING ACTIONS
		auto set(int location, int player) -> void;			// Set's the board with the player's tile
		auto unset(int location, int player) -> void;		// removes the player's tile from the board

		// CONST QUERY ACTIONS
		auto player_at_location(int player, int location) const -> bool;	// Tells you whether player occupies tile
		auto which_player_at(int location) const -> int;					// Tells you which player occupies tile, -1 if none
		auto num_players() const -> int;
		auto num_spaces() const -> int;
		auto potholes() const -> BitBoard { return potholes_; }
		
		auto player_tiles(int player) const -> BitBoard;												// Gives you all 'set' tiles for player
		auto opponent_tiles(int player) const -> BitBoard;												// Gives you all 'set' tiles for opponents
		auto free_tiles() const -> BitBoard;															// Gives all available free tiles on board
		auto free_tiles(int tile, axial::vector const& unit_direction) const -> BitBoard;						// Gives all free tiles along a direction
		auto free_tiles(axial::vector const& anchor_point, axial::vector const& unit_direction) const -> BitBoard;	// Gives all free tiles along a direction
		auto all_boards() const -> std::vector<BitBoard>;

		friend auto operator<<(std::ostream& os, Board const& board) -> std::ostream& {
			auto const board_width = 9;	// Static board size assumption

			auto tile_lookup = [board, board_width](int row, int col, int triangle_offset) {
				auto position = row * 9 + col - triangle_offset;	// Where 9 is the board with
				if (board.potholes().isSet(position) == true) return '\'';
				auto player = board.which_player_at(position);
				if (player == 0) return 'o';
				if (player == 1) return 'x';
				if (player == 2) return '&';

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