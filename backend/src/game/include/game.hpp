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
		std::vector<std::string> move_sequence_;
		// TODO: For backend, need to be able to store the following information for replay functionality.
		// REPLAY SEQUENCE-> "E3A2E7 ....."
		// REPLAY SNAPSHOT-> std::vector<uint64_t>{}

		// P0	0 - (64bit board state)
		// P1	1 - (64bit board state)
		// P2	2 - (64bit board state)
		// P0	3 - (64bit board state)
		// P1	4 - (64bit board state)
		// P2	5 - (64bit board state)
		// P0	6 - (64bit board state)

	public:
		Game(int nplayers);

		auto play(std::string move) -> bool override;	// Does provide error checking
		auto play(int index) -> bool override;			// Does NOT provide error checking

		auto ongoing() const -> bool;
		auto status() const -> Game::state const;		// Tells you whether the game is over or not

		auto move_sequence() const -> std::string;
		auto all_moves() const -> const std::vector<std::string>;
		// auto give_replay() -> 
		// auto check() -> bool; 							// TODO: IMPLEMENT -> Valid move?
		// auto over() -> bool; 							// TODO: IMPLEMENT -> Has the game ended?

	private:
		auto end_turn(int index) -> void;
		auto append_move(std::string move) -> void;
};

#endif
