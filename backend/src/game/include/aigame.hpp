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
		enum class terminal {
			PLAYER0_WIN, PLAYER0_LOSS,
			PLAYER1_WIN, PLAYER1_LOSS,
			NONE,
		};
	private:
		int move_;
		terminal reason_;
		int eval_depth_;	// Says whether the board has been heuristically evaluated
		int score_;			// The heuristic score
		std::vector<std::vector<BitBoard>> states_;
		// int difficulty_;

	public:
		// AIGame(): BaseGame{2}, move_{-1}, reason_{terminal::NONE}, eval_depth_{-1}, score_{0}, states_{}{}	// For the memo class
		AIGame(int nplayers, BitBoard potholes = BitBoard());
		AIGame(AIGame const& position, int move);

		~AIGame() = default;

		auto play(std::string move) -> bool override;	// Does NOT provide error checking
		auto play(int index) -> bool override;			// Does NOT provide error checking
		auto unplay(int index) -> void;

		auto setEval(int depth) -> void {eval_depth_ = depth;}
		auto eval(int depth) const -> bool {
			if (depth == eval_depth_) return true;
			return false;
		}
		auto evalDepth() const -> int {return eval_depth_;}

		auto score_position(int player_perspective, int offset=0) -> int;

		auto store_game(std::vector<BitBoard> const& board) -> void;
		auto generate_all_moves(Memo &memo) -> void;
		
		// auto find_best() -> AIGame&;
		// auto find_worst() -> AIGame&;

		auto move() const -> int;
		auto isTerminal() const -> bool;
		auto reason() const -> AIGame::terminal;
		auto score() const -> int;
		auto states() -> std::vector<std::vector<BitBoard>>&;
		auto states() const -> std::vector<std::vector<BitBoard>> const&;


		auto clear() -> void {
			states_ = std::vector<std::vector<BitBoard>>{};
		}

		auto minmax(int depth, int difficulty) -> int;

	private:
		auto run_minmax(int depth, int player, Memo &memo, int alpha, int beta) -> int;
		int heuristic(int const player);

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

		auto insert_if_not_stored(AIGame const& game, int move) -> void {
			if (this->contains(game.board().all_boards())) {
				// ALREADY INSERTED
				return;
			}
			this->insert(game, move);
		}

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
		auto find(std::vector<BitBoard> const& boards) -> AIGame *{
			return map_.find(boards)->second.get();
		}
};


#endif