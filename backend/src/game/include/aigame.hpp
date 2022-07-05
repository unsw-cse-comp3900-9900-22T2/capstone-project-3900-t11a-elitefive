#ifndef YAVALATH_AIGAME_HPP
#define YAVALATH_AIGAME_HPP

#include <string.h>
#include <vector>
#include <algorithm>
#include <random>
#include <memory>
#include <unordered_map>

#include "basegame.hpp"
// #include "memo.hpp"

class AIGame;
class Memo;

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
		AIGame(): BaseGame{2}, move_{-1}, terminal_{false}, score_{0}, states_{}{}	// For the memo class
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

		auto minmax(int depth) -> int;

	private:
		auto run_minmax(int depth, int for_player) -> int;
		auto heuristic(int const player) -> int;

		auto end_turn() -> void;
		auto undo_turn() -> void;
};

class Memo {
	struct BoardHash {
		std::size_t operator()(std::vector<BitBoard> const& key) const {
			BitBoard seed = BitBoard();
			for (auto const& i : key) {
				seed = seed | i;
			}
			return seed.value();
		}
	};

	struct BoardEqual {
		bool operator()(std::vector<BitBoard> const& lhs, std::vector<BitBoard> const& rhs) const {	
			if (lhs.size() != rhs.size()) return false;
			return std::equal(lhs.begin(), lhs.end(), rhs.begin());
		}
	};
	private:
		std::unordered_map<std::vector<BitBoard>, std::unique_ptr<AIGame>, BoardHash, BoardEqual> map_;
	public:
		Memo() 
		: map_{} 
		{}

		auto insert(AIGame const& game, int move) -> void {
			map_.insert({
				game.board().all_boards(),
				std::make_unique<AIGame>(game, move)
			});
		}

		auto contains(std::vector<BitBoard> const& boards) const -> bool {
			return (map_.find(boards) != map_.end());
		}

		// NOTE: Need to check if it contains first
		auto find(std::vector<BitBoard> const& boards) -> AIGame& {
			return *map_.find(boards)->second;
		}
};


#endif