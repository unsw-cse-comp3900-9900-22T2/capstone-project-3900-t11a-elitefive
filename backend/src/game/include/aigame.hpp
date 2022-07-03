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

		auto move() const -> int;
		auto terminal() const -> bool;
		auto score() const -> int;
		auto states() -> std::vector<AIGame>&;
		auto states() const -> std::vector<AIGame> const&;


		auto clear() -> void {
			states_ = std::vector<AIGame>{};
		}

		auto generate_to_depth(int depth) -> void {
			if (this->terminal()) return; // Do not expand this node
		
			if (depth == 0) {
				// std::cout << *this << '\n';
				this->score_ = this->heuristic();
				return;
			}

			this->generate_all_moves();
			for (auto &state : states_) {
				state.generate_to_depth(depth - 1);
			}

			return;
		}

		auto minmax(int depth) -> int {
			// auto const selections = this->board().free_tiles().binary_to_vector();
			// auto random_selection = std::vector<int>{};
			// std::sample(
			// 	selections.begin(),
			// 	selections.end(),
			// 	std::back_inserter(random_selection),
			// 	1,
			// 	std::mt19937{std::random_device{}()}
			// );
			// std::cout << "Random selection: " << random_selection.front() << '\n';

			auto &best = this->states()[0];
			auto score = AIGame::MINSCORE;

			for (auto &position : this->states()) {
				auto eval = position.run_minmax(depth - 1, this->whose_turn());
				if (eval > score) {
					score = eval;
					best = position;
				}
			}

			// auto score = this->run_minmax(depth, this->whose_turn());
			// AIGame &best = this->find_best();
			// std::cout << "score GENERATED: " << score << '\n';

			// if (best.move() == 0) {
			// 	std::cout << "\tMade a random move\n"; 
			// 	return random_selection.front();
			// }

			return best.move();
		}

		auto run_minmax(int depth, int player) -> int {
			if (depth == 0 || this->terminal()) {
				// std::cout << "\nWHOSE TURN " << this->whose_turn() << " Player: " << player << '\n';
				// std::cout << *this << score() << " Heuristic: " << this->heuristic() << '\n';
				// if (score_ == AIGame::MAXSCORE) {
				// 	exit(1);
				// }
				if (whose_turn() == player) return -this->score();
				return this->score();
			}
			
			if (player == this->whose_turn()) {
				auto score = AIGame::MINSCORE;
				for (auto &position : this->states()) {
					auto eval = position.run_minmax(depth - 1, player);
					if (eval > score) {
						score = eval;
					}
				}
				this->score_ = score;
				return score;
			}
			else {
				auto score = AIGame::MAXSCORE;
				for (auto &position : this->states()) {
					auto eval = position.run_minmax(depth - 1, player);
					if (eval < score) {
						score = eval;
					}
				}
				this->score_ = score;
				return score;
			}
		}

		auto isSet(BitBoard const& board, axial::vector const& vec) const -> bool {
			auto const tile = axial::vector::index(vec);
			if (board.isSet(tile)) return true;
			return false;
		}

		auto heuristic() -> int {
			// Assume current player is trying to win
			Board const &board = this->board();
			auto const player = this->previous_player();
			auto const free_spaces = board.free_tiles();
			auto const player_tiles = board.player_tiles(player);
			auto const opponent_tiles = board.opponent_tiles(player);
			// std::cout << board << '\n';
			// std::cout << opponent_tiles << '\n';
			// for (auto t : opponent_tiles.binary_to_vector()) {
			// 	std::cout << t << '\n';
			// }
			// exit(1);
			// auto tri = axial::vector::unit_directions();
			// for (auto &i : tri) { i *= 3; }

			auto const units = axial::vector::unit_directions();
			auto count = 0;

			std::vector<int> const& tiles = player_tiles.binary_to_vector();
			for (auto const& tile : tiles) {
				// std::cout << tile << '\n';
				for (auto const& dir : units) {
					// Make sure tile is INBOUNDS
					auto const target_vec = dir * 3;
					auto const vec_location = axial::vector(tile) + target_vec;
					// Doesn't land on target
					if (vec_location.distance() > 4) continue; // Ignore tiles outside the board
					
					// std::cout << "\ttarvec: " << target_vec << "  :" << axial::vector::index(target_vec) << '\n';
					// std::cout << "\tvecloc: " << vec_location << "  :" << axial::vector::index(vec_location) << '\n';

					if (!isSet(player_tiles, vec_location)) 		continue; // You don't have a tile here
					// std::cout << "\tHERE " << axial::vector::index(vec_location) << '\n';
					if (isSet(opponent_tiles, (tile + dir))) 		continue; // Opponent tile blocks
					if (isSet(opponent_tiles, tile + (dir * 2))) 	continue; // Opponent tile blocks
					
					if (isSet(player_tiles, vec_location)) ++count; // Weight progress higher
					if (isSet(player_tiles, vec_location)) ++count; // Weight progress higher

					// auto const space_1 = axial::vector::index(axial::vector::index(dir * 2));
					// if (!std::binary_search(free_spaces.begin(), free_spaces.end(), space_1)) continue;
					// auto const space_2 = axial::vector::index(axial::vector::index(dir * 3));
					// if (!std::binary_search(free_spaces.begin(), free_spaces.end(), space_2)) continue;

					++count; // Potential for a connect 4
				}
			}
			// std::cout << count/2 << "\n\n";
			return count/2;
		}


	private:
		auto end_turn() -> void;
		auto undo_turn() -> void;
};

#endif