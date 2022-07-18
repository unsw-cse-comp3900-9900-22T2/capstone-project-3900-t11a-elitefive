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

	public:
		Game(int nplayers);
		Game(int nplayers, std::vector<int> const uids);

		auto play(std::string move) -> bool override;	// Does provide error checking
		auto play(int index) -> bool override;			// Does NOT provide error checking

		auto ongoing() const -> bool;
		auto status() const -> Game::state const;		// Tells you whether the game is over or not

		auto move_sequence() const -> std::string;
		auto all_moves() const -> const std::vector<std::string>;

		auto which_player_won() const -> int {
			auto const state = this->status();
			if (state == Game::state::DRAW) return -1; // No one won. Assume draw in this case
			
			// Game end on player 0's turn
			if (this->whose_turn() == 0) {
				if (state == Game::state::WIN) return 0;	// Player 0 won
				if (state == Game::state::LOSS) return 1;	// Player 1 won
			}
			if (this->whose_turn() == 1) {
				if (state == Game::state::WIN) return 1;	// Player 1 won
				if (state == Game::state::LOSS) return 0;	// Player 0 won
			}
			
			return -1; // No one won. Assume draw in this case
		}

		auto give_uid(int player) const -> int {
			return uids_[player];
		}

		// auto give_replay() -> 
		// auto check() -> bool; 							// TODO: IMPLEMENT -> Valid move?
		// auto over() -> bool; 							// TODO: IMPLEMENT -> Has the game ended?

	private:
		auto end_turn(int index) -> void;
		auto append_move(std::string move) -> void;
};

#endif
