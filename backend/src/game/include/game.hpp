#ifndef YAVALATH_GAME_HPP
#define YAVALATH_GAME_HPP

#include <string.h>

#include "board.hpp"

class Game : public Board {
	public:
		enum class state { ONGOING, WIN, LOSS, DRAW };
		auto static indexToCoord(int index) -> std::string {
			int q = calculate_q(index);
			char letter = 'a' + q + 4;
			int column = index - floor_index(q) + 1;
			return letter + std::to_string(column);
		}

		auto static coordToIndex(std::string coord) -> int {
            int letter = coord[0];
            if (letter >= 'a' && letter <= 'i') {
                return floor_index(letter - 'a' - 4) + stoi(coord.substr(1)) - 1;
            }
            return -1;
        }

	private:
		int player_turn_;
		int nmoves_;
		int total_spaces_;
		std::vector<int> uids_;
		Game::state gamestate_;
	public:
		Game(int nplayers);

		auto play(std::string move) -> bool;
		auto play(int index) -> bool;
		auto status() -> Game::state;
		auto whose_turn() -> int;

		auto num_moves() -> int;

		auto check() -> bool; // Valid move?
		auto over() -> bool; // Has the game ended?

	private:
		auto end_turn(int index) -> void;
		auto won(int move, int player) -> bool;
		auto loss(int move, int player) -> bool;

		auto static floor_index(int q) -> int {
			return ((q + 8) * (q + 9) / 2) - 10 - ((q > 1) ? q * (q - 1) : 0);
		}

		int static calculate_q(int index) {
			for (int q = -4; q < 5; ++q) {
				if (index < floor_index(q + 1)) {
					return q;
				}
			}
			return -1;
		}

};

#endif
