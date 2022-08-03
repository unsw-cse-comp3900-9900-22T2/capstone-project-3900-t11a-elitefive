#ifndef COMPUTER_BOARD_HPP
#define COMPUTER_BOARD_HPP

#include "game.hpp"

class Computer {
	private:
		Game game_;
	public:
		Computer(Game game);
		auto make_random_move() const -> int;
		auto make_random_move(Board const& board) const -> int;
};

#endif