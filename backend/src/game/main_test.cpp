#include <vector>
#include <iostream>

#include "board.hpp"
#include "computer.hpp"
#include "aigame.hpp"
#include "game.hpp"
#include "bitboard.hpp"


auto main(void) -> int {

	auto game = AIGame(2);
	game.play(1);
	game.play(14);
	game.play(3);
	game.play(11);
	game.play(4);
	game.play(13);
	game.play(5);

	
	std::cout << game << '\n';

	game.generate_all_moves();
	game.find_best();
	// std::cout << game << '\n';

	return 0;
}