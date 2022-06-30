#ifndef YAVALATH_BASEGAME_HPP
#define YAVALATH_BASEGAME_HPP

#include <vector>

#include "bitboard.hpp"
#include "board.hpp"

class BaseGame {
    private:
        Board board_;
		int player_turn_;
		int nmoves_;
    public:
        BaseGame(int players);
        virtual ~BaseGame() = default;

        auto virtual play(std::string move) -> bool = 0;
		auto virtual play(int index) -> bool = 0;
		
		auto whose_turn() -> int;
		auto num_moves() -> int;
        auto board() -> Board &;             // Normally you can just change the board
        auto board() const -> Board const&;  // Needed for the '<<' operator


        // auto status() -> Game::state;
        auto increase_move() -> void;
        auto pass_turn() -> void;

		auto check() -> bool; // Valid move?
		auto over() -> bool; // Has the game ended?

        friend auto operator<<(std::ostream& os, BaseGame const& basegame) -> std::ostream& {
            os << basegame.board();
            return os;
        }

    private:
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
    public:
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
};

#endif