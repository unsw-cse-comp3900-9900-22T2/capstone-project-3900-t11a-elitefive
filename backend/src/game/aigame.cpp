#include <string.h>

#include "aigame.hpp"
#include "basegame.hpp"

AIGame::AIGame(int nplayers)
: BaseGame{nplayers}
, terminal_{false}
{}

AIGame::AIGame(int nplayers, bool terminal)
: BaseGame{nplayers}
, terminal_{terminal}
{}

auto AIGame::play(std::string move) -> bool {
	auto position = BaseGame::coordToIndex(move);
	return play(position);
}

auto AIGame::play(int index) -> bool {
	board().set(index, this->whose_turn());
	end_turn();
	return true;
}

auto AIGame::unplay(int index) -> void {
	undo_turn();
	board().unset(index, this->whose_turn());
}

auto terminal() -> bool;


auto AIGame::end_turn() -> void {
	this->increase_move();
	this->pass_turn();
}

auto AIGame::undo_turn() -> void {
	this->decrease_move();
	this->unpass_turn();
}