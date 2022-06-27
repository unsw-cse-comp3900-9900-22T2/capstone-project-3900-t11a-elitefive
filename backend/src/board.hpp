#ifndef YAVALATH_BOARD_HPP
#define YAVALATH_BOARD_HPP

#include <vector>
#include <string>

#include "hexagon.hpp"

class Board {
	private:
		int nplayers_;			// How many players in match
		int player_turn_;		// Current player's turn
		int nmoves_; 			// Total number of moves played
		std::vector<int> uids_; // User IDs
		std::vector<Hexagon> boardstate_;

	public:
		Board(int nplayers, std::vector<int> uids);

		// Plays the move
		auto play_move(std::string tile) -> bool;

		// Simple getters
		auto num_players() -> int {return nplayers_;};
		auto whose_turn() -> int {return player_turn_;};
		auto num_moves() -> int {return nmoves_;};
};


#endif