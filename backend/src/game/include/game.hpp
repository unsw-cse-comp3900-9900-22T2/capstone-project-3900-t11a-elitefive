#ifndef YAVALATH_GAME_HPP
#define YAVALATH_GAME_HPP

#include <string.h>
#include "basegame.hpp"
#include "bitboard.hpp"

class Game : public BaseGame {
	public:
		enum class state { ONGOING, WIN, LOSS, DRAW };
	private:
		std::vector<int> uids_; 
		std::vector<bool> out_;
		Game::state gamestate_;
		std::vector<std::string> move_sequence_;
	public:
		Game(int nplayers, BitBoard potholes = BitBoard());
		Game(int nplayers, std::vector<int> const uids, BitBoard potholes = BitBoard());

		auto play(std::string move) -> bool override;	// Does provide error checking
		auto play(int index) -> bool override;			// Does NOT provide error checking

		auto ongoing() const -> bool;
		auto status() const -> Game::state const;		// Tells you whether the game is over or not
		auto move_sequence() const -> std::string;
		auto all_moves() const -> const std::vector<std::string>;

		auto num_players_in() -> int {
			int count = 0;
			for (auto const value : out_) {
				if (value == false) ++count;
			}
			return count;
		}
		auto set_player_out(int player) -> void {out_[player] = true; }
		auto continue_game() -> void {gamestate_ = Game::state::ONGOING;}

		auto which_player_won() const -> int {
			auto const state = this->status();
			if (state == Game::state::DRAW) return -1; // No one won. Assume draw in this case
			
			// Game end on player 0's turn
			if (this->num_players() == 2) {
				if (this->whose_turn() == 0) {
					if (state == Game::state::WIN) return 0;	// Player 0 won
					if (state == Game::state::LOSS) return 1;	// Player 1 won
				}
				if (this->whose_turn() == 1) {
					if (state == Game::state::WIN) return 1;	// Player 1 won
					if (state == Game::state::LOSS) return 0;	// Player 0 won
				}
			}
			else {
				if (this->whose_turn() == 0) {
					if (state == Game::state::WIN) 						return 0;	// Player 0 won
					if (state == Game::state::LOSS && out_[2] == true) 	return 1;	// Player 1 won - Player 2 out
					if (state == Game::state::LOSS && out_[1] == true) 	return 2;	// Player 2 won - Player 1 out
				}
				if (this->whose_turn() == 1) {
					if (state == Game::state::WIN) 						return 1;	// Player 1 won
					if (state == Game::state::LOSS && out_[2] == true) 	return 0; 	// Player 0 won - Player 2 out
					if (state == Game::state::LOSS && out_[0] == true) 	return 2;	// Player 2 won - Player 0 out
				}
				if (this->whose_turn() == 2) {
					if (state == Game::state::WIN) 						return 2;	// Player 2 won
					if (state == Game::state::LOSS && out_[1] == true) 	return 0; 	// Player 0 won - Player 1 out
					if (state == Game::state::LOSS && out_[0] == true) 	return 1;	// Player 1 won - Player 0 out
				}
			}
			
			return -1; // No one won. Assume draw in this case
		}

		auto give_uid(int player) const -> int {
			return uids_[player];
		}

		auto give_player_with_uid(int uid) const -> int {
			for (int i = 0; i < num_players(); ++i) {
				if (give_uid(i) == uid) return i;
			}
			return -1; // This uid is not playing
		}

		// auto give_replay() -> 
		// auto check() -> bool; 							// TODO: IMPLEMENT -> Valid move?
		// auto over() -> bool; 							// TODO: IMPLEMENT -> Has the game ended?

	private:
		auto end_turn(int index) -> void;
		auto append_move(std::string move) -> void;
};

#endif
