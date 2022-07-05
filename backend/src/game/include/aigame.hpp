#ifndef YAVALATH_AIGAME_HPP
#define YAVALATH_AIGAME_HPP

#include <string.h>
#include <vector>
#include <algorithm>
#include <random>

#include "basegame.hpp"

class AIGame : public BaseGame {
	public:
		static int const MAXSCORE = 999;
		static int const MINSCORE = -999;
		static int const PLAYER0 = 1000;	// WINNER TODO: Hacky
		static int const PLAYER1 = 1001;	// WINNER TODO: Hacky

	private:
		int move_;
		bool terminal_;
		int score_;
		std::vector<AIGame> states_;
	public:
		AIGame(int nplayers);
		AIGame(AIGame const& position, int move);

		~AIGame() = default;

		auto play(std::string move) -> bool override;	// Does NOT provide error checking
		auto play(int index) -> bool override;			// Does NOT provide error checking
		auto unplay(int index) -> void;
		
		auto store_game(AIGame game) -> void;
		auto generate_all_moves() -> void;
		
		auto find_best() -> AIGame&;
		auto find_worst() -> AIGame&;

		auto move() const -> int;
		auto terminal() const -> bool;
		auto score() const -> int;
		auto states() -> std::vector<AIGame>&;
		auto states() const -> std::vector<AIGame> const&;


		auto clear() -> void {
			states_ = std::vector<AIGame>{};
		}

		auto minmax(int depth) -> int {
			return run_minmax(depth, this->whose_turn());
		}

	private:
		auto run_minmax(int depth, int for_player) -> int;
		auto heuristic(int const player) -> int;

		auto end_turn() -> void;
		auto undo_turn() -> void;
};

#endif