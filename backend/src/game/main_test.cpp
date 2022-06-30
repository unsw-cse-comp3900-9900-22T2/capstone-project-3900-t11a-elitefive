#include <vector>
#include <iostream>

#include "board.hpp"
#include "computer.hpp"
#include "game.hpp"

auto main(void) -> int {
	auto game = Game(2);

	for (int i = 0; i < 10 ; ++i) {
		int move = -1;
		do {
			// std::cout << game << '\n';
			Computer computer(game);
			move = computer.make_random_move();
		} while (game.play(Game::indexToCoord(move)));
		std::cout << game << '\n';
	}
}