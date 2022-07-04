#include <string.h>
#include <vector>

#include "aigame.hpp"
#include "basegame.hpp"

AIGame::AIGame(int nplayers)
: BaseGame{nplayers}
, move_{-1}
, terminal_{false}
, score_{0}		// TODO: CHANGE TO UNSET
, states_{}
{}

AIGame::AIGame(AIGame const& position, int move)
: BaseGame{position.board(), position.whose_turn(), position.num_moves()}
, move_{move}
, terminal_{position.terminal()}
, score_{position.score()}		// TODO: CHANGE FOR NO SCORE
, states_{}
{}

auto AIGame::play(std::string move) -> bool {
	auto position = BaseGame::coordToIndex(move);
	return play(position);
}

auto AIGame::generate_all_moves() -> void {
	auto const previous_score = score();
	auto const nplayers = board().num_players();
	for (auto const& move : board().free_tiles().binary_to_vector()) {
		play(move);
		store_game(AIGame(*this, move));
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
		score_ = set_end_state(curr_player);
	} else if (loss(index, curr_player)) {
		terminal_ = true;
		score_ = -set_end_state(curr_player);
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

auto AIGame::states() -> std::vector<AIGame>& {
	return states_;
}

auto AIGame::store_game(AIGame game) -> void {
	states_.push_back(game);
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

auto AIGame::states() const -> std::vector<AIGame> const& {
	return states_;
}

auto AIGame::find_best() -> AIGame& {
	// TODO: If it's all a tie, pick from the bucket of ties instead of defaulting to start
	auto const &index = std::max_element(states().begin(), states().end(), [](AIGame const& a1, AIGame const& a2) {
		return a1.score_ < a2.score_;
	});
	// std::cout << *index << '\n';
	// return index->score_;
	return *index;
}

auto AIGame::find_worst() -> AIGame& {
	// TODO: If it's all a tie, pick from the bucket of ties instead of defaulting to start
	auto const &index = std::min_element(states().begin(), states().end(), [](AIGame const& a1, AIGame const& a2) {
		return a1.score_ < a2.score_;
	});
	// std::cout << *index << '\n';
	// return index->score_;
	return *index;
}