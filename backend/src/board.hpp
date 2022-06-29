#ifndef YAVALATH_BOARD_HPP
#define YAVALATH_BOARD_HPP

#include <vector>
#include <string>

#include "hexagon.hpp"

class Board {
	public:
		enum class state { ONGOING, WIN, LOSS, DRAW };
	private:
		int nplayers_;			// How many players in match
		int player_turn_;		// Current player's turn
		int nmoves_; 			// Total number of moves played
		int total_spaces_;		// Total number available hexes from start
		std::vector<int> uids_; // User IDs
		std::vector<Hexagon> boardstate_;
		Board::state gamestate_;
	public:
		Board(int nplayers, std::vector<int> uids);

		// Game engine functions
		auto play_move(std::string move) -> bool;
		auto play_move(Hexagon &hex) -> bool;
		auto is_valid_move(std::string move) -> bool;
		auto is_valid_move(Hexagon &hex) -> bool;

		auto game_status() -> Board::state const& {return gamestate_;};
		auto view_available_tiles() const -> std::vector<Hexagon> const;
		auto is_available_tile(Hexagon &hex) -> bool;

		auto find_tile(int index) -> Hexagon &;
		auto find_tile(Hexagon hex) -> Hexagon & {
			// TODO -> Remove this dump stupid hack because of view_available_tiles()
			return boardstate_[hex.tileLocation()];
		}

		// Simple getters
		auto num_players() -> int {return nplayers_;};
		auto whose_turn() -> int {return player_turn_;};
		auto num_moves() -> int {return nmoves_;};
		auto view_tile(int index) const -> Hexagon const& { return boardstate_[index];}

		// TODO: Perhaps these two should be moved elsewhere. Perhaps static too?
		static auto display_coord_to_flatten_index(std::string s) -> int {
			int letter = s[0];
			if (letter >= 'a' && letter <= 'i') {
				int row = letter - 'a';
				int column = stoi(s.substr(1)) - 1;
				int index = (row <= 4 ? (row * 9) : (row - 4) * 10 + 36) + column;
				return index;
			}
			return -1;
		}

		static auto flatten_index_to_display_coord(int i) -> std::string {
			int row = i / 9;
			int column = (i % 9) + 1 - ((i < 45) ? 0 : row - 4);
			char letter = 'a' + row;
			return letter + std::to_string(column);
		}

		friend auto operator<<(std::ostream& os, Board const& board) -> std::ostream& {
			auto const board_width = 9;	// Static board size assumption

			auto tile_lookup = [board, board_width](int row, int col) {
				auto position = row * board_width + col;
				if (board.view_tile(position).getPlayer() == 0) return 'o';
				if (board.view_tile(position).getPlayer() == 1) return 'x';
				return '-';
			};

			for (int row = -4; row <= 4 ; ++row) {
				auto const spacing = (row < 0) ? -row : row;
				for (int i = 0; i < spacing; ++i) {
					os << " ";
				}
				for (int i = 0; i < board_width - spacing; ++i) {
					auto const col = (row < 0) ? 0 : row;
					auto symbol = tile_lookup(row + 4, col + i);
					os << symbol << " ";
				}
				os << '\n';
			}

			return os;
		}

	private:
		// Updates the state of the game and checks if game is over
		auto end_turn(int move) -> void; // Called by play_move
};


#endif