#ifndef COMPUTER_BOARD_HPP
#define COMPUTER_BOARD_HPP

#include "board.hpp"
#include "hexagon.hpp"

class Computer {
	private:
		Board board_;
	public:
		Computer(Board board);
		auto make_random_move() -> Hexagon&;
};

#endif