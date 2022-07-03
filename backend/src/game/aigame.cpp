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

auto AIGame::play(int index) -> bool {
	board().set(index, this->whose_turn());
	if (won(index, this->whose_turn())) {
		terminal_ = true;
		score_ = AIGame::MAXSCORE;
	} else if (loss(index, this->whose_turn())) {
		terminal_ = true;
		score_ = AIGame::MINSCORE;
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