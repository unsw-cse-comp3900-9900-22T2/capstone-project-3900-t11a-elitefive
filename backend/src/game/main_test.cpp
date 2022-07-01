#include <vector>
#include <iostream>

#include "board.hpp"
#include "computer.hpp"
#include "aigame.hpp"
#include "game.hpp"
#include "bitboard.hpp"


auto main(void) -> int {
	// auto game = Game(2);

	// for (int i = 0; i < 10 ; ++i) {
	// 	int move = -1;
	// 	do {
	// 		// std::cout << game << '\n';
	// 		Computer computer(game);
	// 		move = computer.make_random_move();
	// 	} while (game.play(Game::indexToCoord(move)));
	// 	std::cout << game << '\n';
	// }


	// int move = -1;
	// auto game = AIGame(2);
	// game.play(1);
	// game.play(2);
	// game.play(3);
	// std::cout << game << '\n';
	// game.unplay(3);
	// std::cout << game << '\n';
	// // game.unplay(3);
	// game.play(4);
	// std::cout << game << '\n';




	// auto b = BitBoard(); 
	// std::cout << b << '\n';
	// b.set(3);
	// std::cout << b << '\n';
	// b.unset(3);
	// std::cout << b << '\n';

	auto game = AIGame(2);
	game.play(1);
	std::cout << game << '\n';
	for (auto const& i : game.board().free_tiles().binary_to_vector()) {
		game.play(i);
		std::cout << game << '\n';
		game.unplay(i);
	}
	
	// do {
	// 	Computer computer(game);
	// 	move = computer.make_random_move();
	// } while (game.play(Game::indexToCoord(move)));

	return 0;
}