#include <string.h>
#include <vector>
#include <assert.h> // TODO: DELETE
#include "aigame.hpp"
#include "basegame.hpp"

// Helper functions
auto isSet(BitBoard const& board, axial::vector const& vec) -> bool;

AIGame::AIGame(int nplayers)
: BaseGame{nplayers}
, move_{-1}
, terminal_{false}
, score_{0}		// TODO: CHANGE TO UNSET
, eval_{false}
, states_{}
{}

AIGame::AIGame(AIGame const& position, int move)
: BaseGame{position.board(), position.whose_turn(), position.num_moves()}
, move_{move}
, terminal_{position.terminal()}
, score_{position.score()}		// TODO: CHANGE FOR NO SCORE
, eval_{false}
, states_{}
{}

auto AIGame::play(std::string move) -> bool {
	auto position = BaseGame::coordToIndex(move);
	return play(position);
}

auto AIGame::generate_all_moves(Memo &memo) -> void {
	auto const previous_score = score();
	auto const nplayers = board().num_players();
	for (auto const& move : board().free_tiles().binary_to_vector()) {
		play(move);
		store_game(this->board().all_boards());
		memo.insert_if_not_stored(*this, move);
		unplay(move);
	}

	score_ = previous_score;
}

auto set_end_state(int player) {
	if (player == 0) return AIGame::PLAYER0;
	if (player == 1) return -AIGame::PLAYER1;
}

auto AIGame::play(int index) -> bool {
	auto const curr_player = this->whose_turn();
	board().set(index, curr_player);
	if (won(index, curr_player)) {
		terminal_ = true;
		score_ = set_end_state(curr_player); // TODO: Change because it is hacky
	} else if (loss(index, curr_player)) {
		terminal_ = true;
		score_ = -set_end_state(curr_player); // TODO: Change because it is hacky
	}
	end_turn();
	return true;
}

auto AIGame::unplay(int index) -> void {
	undo_turn();
	board().unset(index, this->whose_turn());
}

auto AIGame::terminal() const -> bool {
	return terminal_;
}

auto AIGame::score() const -> int {
	return score_;
}

auto AIGame::states() -> std::vector<std::vector<BitBoard>>& {
	return states_;
}

auto AIGame::store_game(std::vector<BitBoard> const& board) -> void {
	states_.push_back(board);
}

auto AIGame::end_turn() -> void {
	this->increase_move();
	this->pass_turn();
	
}

auto AIGame::move() const -> int {
	return move_;
}

auto AIGame::undo_turn() -> void {
	this->decrease_move();
	this->unpass_turn();
	terminal_ = false;
	score_ = 0;
}

auto AIGame::states() const -> std::vector<std::vector<BitBoard>> const& {
	return states_;
}

// auto AIGame::find_best() -> AIGame& {
// 	// TODO: If it's all a tie, pick from the bucket of ties instead of defaulting to start
// 	auto const &index = std::max_element(states().begin(), states().end(), [](AIGame const& a1, AIGame const& a2) {
// 		return a1.score_ < a2.score_;
// 	});
// 	return *index;
// }

// auto AIGame::find_worst() -> AIGame& {
// 	// TODO: If it's all a tie, pick from the bucket of ties instead of defaulting to start
// 	auto const &index = std::min_element(states().begin(), states().end(), [](AIGame const& a1, AIGame const& a2) {
// 		return a1.score_ < a2.score_;
// 	});
// 	return *index;
// }

auto AIGame::minmax(int depth) -> int {
	auto memo = Memo();
	auto const score = run_minmax(depth, this->whose_turn(), memo, -99999, 99999);
	std::cout << "Best score: " << score << '\n';
	for (auto const& state : this->states()) {
		AIGame *position = memo.find(state);
		if (position->score() == score) return position->move();
	}
	exit(1);
	// return move;
}

// Returns best score
auto AIGame::run_minmax(int depth, int for_player, Memo &memo, int alpha, int beta) -> int {
	if (this->terminal()) {
		auto const player_end = (PLAYER0 == score_ || PLAYER0 == -score_) ? 0 : 1;
		score_ += depth;	// TODO: HACK to avoid BMing the other player. Will take a win at earlier depths
		

		if (player_end != for_player) return -score_;
		return score_;
	}
	// if (this->eval()) {
	// 	// We evaluated a transposition of this position earlier.
	// 	std::cout << "Evaluated already. Depth: " << depth << "\tScore: " << this->score() << '\n';
	// 	return this->score();
	// }
	if (depth == 0) {
		// Generate heuristic
		auto curr = heuristic(for_player);
		auto other = heuristic(this->player_after(for_player));
		
		this->setEval();
		
		// TODO: This needs to change for three player
		score_ = curr - other;		// Get different between both players for eval
		return this->score();
	}
	
	this->generate_all_moves(memo);

	// Player 0 is trying to maximize scores (+ve values)
	if (for_player == 0) {
		// WANT TO MAXIMIZE
		int maxeval = -9999;
		AIGame *ptr = nullptr; 

		for (auto &state : this->states()) {
			AIGame *position = memo.find(state);
			int eval = position->score();
			if (!position->eval()) {
				eval = position->run_minmax(depth - 1, player_after(for_player), memo, alpha, beta);
			}

			if (eval > maxeval) {	// MAX
				maxeval = eval;
				ptr = position;
			}
			alpha = std::max(alpha, maxeval);
			if (beta <= alpha) break;
		}

		// Set the best score
		this->score_ = ptr->score();
		return ptr->score();
	}
	else { // Player 1 is trying to minimize scores (-ve values)
		// WANT TO MINIMIZE
		int mineval = 9999;
		AIGame *ptr = nullptr; 

		for (auto &state : this->states()) {
			AIGame *position = memo.find(state);
			int eval = position->score();
			if (!position->eval()) {
				eval = position->run_minmax(depth - 1, player_after(for_player), memo, alpha, beta);
			}

			if (eval < mineval) {	// MIN
				mineval = eval;
				ptr = position;
			}
			beta = std::min(beta, mineval);
			if (beta <= alpha) break;
		}

		// Set the worst score
		this->score_ = ptr->score();
		return ptr->score();
	}
}


auto AIGame::heuristic(int const player) -> int {
	// Assume current player is trying to win
	Board const &board = this->board();
	auto const free_spaces = board.free_tiles();
	auto const player_tiles = board.player_tiles(player);
	auto const opponent_tiles = board.opponent_tiles(player);

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

			if (!isSet(player_tiles, vec_location)) 		continue; // You don't have a tile here
			if (isSet(opponent_tiles, (tile + dir))) 		continue; // Opponent tile blocks
			if (isSet(opponent_tiles, tile + (dir * 2))) 	continue; // Opponent tile blocks
			
			if (isSet(player_tiles, (tile + dir))) ++count; 	// Weight progress higher
			if (isSet(player_tiles, tile + (dir * 2))) ++count; // Weight progress higher

			++count; // Potential for a connect 4
		}
	}
	return count/2;
}


// Helper functions
auto isSet(BitBoard const& board, axial::vector const& vec) -> bool {
	auto const tile = axial::vector::index(vec);
	if (board.isSet(tile)) return true;
	return false;
}