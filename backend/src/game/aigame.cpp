#include <string.h>
#include <vector>
#include <assert.h> // TODO: DELETE
#include <algorithm>
#include "aigame.hpp"
#include "basegame.hpp"

// Helper functions
auto isSet(BitBoard const& board, axial::vector const& vec) -> bool;

AIGame::AIGame(int nplayers, BitBoard potholes)
: BaseGame{nplayers, potholes}
, move_{-1}
, reason_{AIGame::terminal::NONE}
, eval_depth_{-1}
, score_{0}		// TODO: CHANGE TO UNSET
, states_{}
// , difficulty_{difficulty}
{}

AIGame::AIGame(AIGame const& position, int move)
: BaseGame{position.board(), position.whose_turn(), position.num_moves()}
, move_{move}
, reason_{position.reason()}
, score_{position.score()}		// TODO: CHANGE FOR NO SCORE
, eval_depth_{position.evalDepth()}
, states_{}
// , difficulty_{difficulty}
{}

auto AIGame::play(std::string move) -> bool {
	auto position = BaseGame::coordToIndex(move);
	return play(position);
}

auto AIGame::generate_all_moves(Memo &memo) -> void {
	if (states_.size() != 0) return; // Already generated
	auto const previous_score = score();
	// auto const nplayers = board().num_players();
	for (auto const& move : board().free_tiles().binary_to_vector()) {
		play(move);
		store_game(this->board().all_boards());
		memo.insert_if_not_stored(*this, move);
		unplay(move);
	}

	score_ = previous_score;
}

auto AIGame::play(int index) -> bool {
	auto const curr_player = this->whose_turn();
	board().set(index, curr_player);
	if (won(index, curr_player)) {
		if (curr_player == 0) reason_ = AIGame::terminal::PLAYER0_WIN;
		if (curr_player == 1) reason_ = AIGame::terminal::PLAYER1_WIN;

	} else if (loss(index, curr_player)) {
		if (curr_player == 0) reason_ = AIGame::terminal::PLAYER0_LOSS;
		if (curr_player == 1) reason_ = AIGame::terminal::PLAYER1_LOSS;
	}
	end_turn();
	return true;
}

auto AIGame::unplay(int index) -> void {
	undo_turn();
	board().unset(index, this->whose_turn());
}

auto AIGame::isTerminal() const -> bool {
	if (this->num_moves() == this->board().num_spaces()) { 
		return true; // Terminal - Draw position
	}	
	return (reason_ != AIGame::terminal::NONE);
}

auto AIGame::reason() const -> AIGame::terminal {
	return reason_;
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
	reason_ = AIGame::terminal::NONE;
	score_ = 0;		// TODO: Could be a problem
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

auto AIGame::minmax(int depth, int difficulty) -> int {
	auto memo = Memo();
	int final_move = -1;
	std::cout << "Minmax at: " << this->num_moves() << '\n';
	// if (depth > 6 && this->num_moves() <= 31) depth = 6;
	// if (depth > 5 && this->num_moves() <= 25) depth = 5;
	// if (depth > 4 && this->num_moves() <= 17) depth = 4;
	// if (depth > 3 && this->num_moves() <= 5) depth = 3;
	for (int inc_depth = depth; inc_depth <= depth; ++inc_depth) {
		std::cout << "\tCalculating depth: " << inc_depth << ' ';
		auto const score = run_minmax(inc_depth, this->whose_turn(), memo, -99999, 99999);
		for (auto const& state : this->states()) {
			AIGame *position = memo.find(state);
			// std::cout << *position << position->move() << '\t' << position->score() << '\n';
			if (position->score() == score) {
				// std::cout << position->move() << '\n';
				final_move = position->move();
				break;
			}
		}
		std::cout << "\tDepth: " << inc_depth << "\tBest score: " << score << "\tMove: " << final_move << '\n';
		if (std::abs(score) >= 1000) break;
		// if (this->num_moves() <= 2) break;
	}
	// exit(1);
	return final_move;
}

auto AIGame::score_position(int player_perspective, int offset) -> int /*score of state*/ {
	if (this->isTerminal() == true) {
		auto const& reason = this->reason();
		switch(reason) {
			case terminal::PLAYER0_WIN:		return  1000 + offset;	
			case terminal::PLAYER1_WIN:		return -1000 - offset;
			case terminal::PLAYER0_LOSS:	return -1000 - offset;
			case terminal::PLAYER1_LOSS:	return  1000 + offset;
		}
	}

	auto p0 = heuristic(0);	// Player 0
	auto p1 = heuristic(1);	// Player 1
	return p0 - p1;

}


// Returns best score
auto AIGame::run_minmax(int depth, int player, Memo &memo, int alpha, int beta) -> int /*score of state*/ {
	if (depth == 0 || this->isTerminal()) {
		auto score = this->score_position(player, depth);
		this->score_ = score;
		this->setEval(depth);
		return score;
	}

	this->generate_all_moves(memo);

	if (player == 0) {
		int maxEval = -99999;
		AIGame *best = nullptr;

		for (auto const& board_id : this->states()) {
			AIGame *state = memo.find(board_id);
			int eval = state->score(); 		// If precomputed take existing eval...
			if (state->eval(depth) == false) {	// Need to compute for this state
				eval = state->run_minmax(depth - 1, player_after(player), memo, alpha, beta);
				state->setEval(depth);
			}
			else {
				// std::cout << "Precomputed\n";
			}
			if (eval > maxEval) {
				maxEval = eval;
				best = state;
			}
			if (eval >= beta) break;
			alpha = std::max(alpha, eval);
		}

		// Sort higher scoring positions first
		std::stable_sort(states_.begin(), states_.end(), [&memo](std::vector<BitBoard> const&a, std::vector<BitBoard> const&b) {
			AIGame *s1 = memo.find(a);
			AIGame *s2 = memo.find(b);
			return s1->score() > s2->score();
		});
		
		this->score_ = best->score();
		return this->score_;
	}

	if (player == 1) {
		int minEval = 99999;
		AIGame *worst = nullptr;

		for (auto const& board_id : this->states()) {
			AIGame *state = memo.find(board_id);
			int eval = state->score(); 		// If precomputed take existing eval...
			if (state->eval(depth) == false) {	// Need to compute for this state
				eval = state->run_minmax(depth - 1, player_after(player), memo, alpha, beta);
				state->setEval(depth);
			}
			if (eval < minEval) {
				minEval = eval;
				worst = state;
			}
			if (eval <= alpha) break;
			beta = std::min(beta, eval);
		}

		// Sort lowest scoring positions first
		std::stable_sort(states_.begin(), states_.end(), [&memo](std::vector<BitBoard> const&a, std::vector<BitBoard> const&b) {
			AIGame *s1 = memo.find(a);
			AIGame *s2 = memo.find(b);
			return s1->score() < s2->score();
		});
		
		this->score_ = worst->score();
		return this->score_;
	}
}

auto straight_line(axial::vector const& dir, int tile, BitBoard const& player_tiles, BitBoard const& opponent_tiles) -> int {
	int count = 0;
	// Make sure tile is INBOUNDS
	axial::vector const target_vec = dir * 3;
	axial::vector const vec_location = axial::vector(tile) + target_vec;
	// Doesn't land on target
	if (vec_location.distance() > 4) return 0; // Ignore tiles outside the board

	if (!isSet(player_tiles, vec_location)) 		return 0; // You don't have a tile here
	if (isSet(opponent_tiles, (tile + dir))) 		return 0; // Opponent tile blocks
	if (isSet(opponent_tiles, tile + (dir * 2))) 	return 0; // Opponent tile blocks
	
	if (isSet(player_tiles, (tile + dir))) ++count; 	// Weight progress higher
	if (isSet(player_tiles, tile + (dir * 2))) count = count + 20; // Weight progress higher

	++count; // Potential for a connect 4
	return count;
}

int AIGame::heuristic(int const player) {
	// Assume current player is trying to win
	Board const &board = this->board();
	BitBoard const free_spaces = board.free_tiles();
	BitBoard const player_tiles = board.player_tiles(player);
	BitBoard const opponent_tiles = board.opponent_tiles(player);

	auto count_lines = 0;
	auto count_distance = 0;

	std::vector<int> const& tiles = player_tiles.binary_to_vector();
	auto const units = axial::vector::unit_directions();
	for (int const& tile : tiles) {
		axial::vector curr_tile = axial::vector(tile);
		for (axial::vector const& dir : units) {
			count_lines += straight_line(dir, tile, player_tiles, opponent_tiles);
		}

		// penalise tile location if it's far away
		if (isSet(player_tiles, tile)) {
			count_distance -= curr_tile.distance();
		}
	}
	return (count_lines/2) + count_distance;
}


// Helper functions
auto isSet(BitBoard const& board, axial::vector const& vec) -> bool {
	auto const tile = axial::vector::index(vec);
	if (board.isSet(tile)) return true;
	return false;
}