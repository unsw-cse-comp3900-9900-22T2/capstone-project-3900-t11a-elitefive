#ifndef YAVALATH_GAME_HPP
#define YAVALATH_GAME_HPP

#include "Board.hpp"

class Game : public Board {
	public:
		enum class state { ONGOING, WIN, LOSS, DRAW };
	private:
		int player_turn_;
		int nmoves_;
		int total_spaces_;
		std::vector<int> uids_;
		Game::state gamestate_;
	public:
		Game(int nplayers);

		auto play(int index) -> bool;
		auto status() -> Game::state;
		auto whose_turn() -> int;

		auto num_moves() -> int;

		auto over() -> bool; // Has the game ended?

	private:
		auto end_turn(int index) -> void;
		auto won(int move, int player) -> bool;
		auto loss(int move, int player) -> bool;

};

#endif
