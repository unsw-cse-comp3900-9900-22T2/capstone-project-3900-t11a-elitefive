#ifndef YAVALATH_BOARD_HPP
#define YAVALATH_BOARD_HPP

#include <vector>
#include "Hexagon.hpp"

class Board {
	private:
		int nplayers_;			// How many players in match
		int player_turn_;		// Current player's turn
		int moves_; 			// Total number of moves played
		std::vector<int> uid_; 	// User IDs
		std::vector<Hexagon> boardstate_;
};


#endif