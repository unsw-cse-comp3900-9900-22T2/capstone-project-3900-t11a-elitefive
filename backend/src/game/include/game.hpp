#ifndef YAVALATH_GAME_HPP
#define YAVALATH_GAME_HPP

#include <string.h>
#include "basegame.hpp"

class Game : public BaseGame {
	public:
		enum class state { ONGOING, WIN, LOSS, DRAW };
	private:
		std::vector<int> uids_;
		Game::state gamestate_;
	public:
		Game(int nplayers);

		auto play(std::string move) -> bool override;	// Does provide error checking
		auto play(int index) -> bool override;			// Does NOT provide error checking

		auto status() -> Game::state;					// Tells you whether the game is over or not

		auto check() -> bool; 							// TODO: IMPLEMENT -> Valid move?
		auto over() -> bool; 							// TODO: IMPLEMENT -> Has the game ended?

	private:
		auto end_turn(int index) -> void;
};

#endif
