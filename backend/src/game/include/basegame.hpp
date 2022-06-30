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
		// Helper static functions that facilate the public static functions
		auto static floor_index(int q) -> int;
		auto static calculate_q(int index) -> int;

	public:
		auto static indexToCoord(int index) -> std::string;
		auto static coordToIndex(std::string coord) -> int;
};

#endif