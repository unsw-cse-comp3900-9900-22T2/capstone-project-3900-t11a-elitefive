#ifndef YAVALATH_AIGAME_HPP
#define YAVALATH_AIGAME_HPP

#include <string.h>
#include <vector>
#include <algorithm>

#include "basegame.hpp"

class AIGame : public BaseGame {
    public:
        static int const MAXSCORE = 999;
        static int const MINSCORE = -999;
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
        
		auto move() const -> int;
        auto terminal() const -> bool;
        auto score() const -> int;
        auto states() -> std::vector<AIGame>&;

        auto heuristic() -> int {
            // Assume current player is trying to win
            Board &board = this->board();
            auto const player = whose_turn();
        }

		auto find_best() -> int {
			auto index = std::max_element(states().begin(), states().end(), [](AIGame const& a1, AIGame const& a2) {
				return a1.score_ < a2.score_;
			});
			std::cout << *index << '\n';
			return index->score_;
		}

    private:
        auto end_turn() -> void;
        auto undo_turn() -> void;
};

#endif