#include <vector>
#include <iostream>

#include "board.hpp"
#include "computer.hpp"
#include "aigame.hpp"
#include "game.hpp"
#include "bitboard.hpp"


auto main(void) -> int {

	// for (int i = 0; i < 10000; ++i) {
	// 	auto game = Game(2);
	// 	auto computer = Computer(game);
	// 	while (game.ongoing()) {
	// 		auto const move = computer.make_random_move(std::as_const(game).board());
	// 		game.play(move);
	// 	}
	// 	// std::cout << game << '\n';
	// }


	auto game = AIGame(2);
	game.play(0);
	game.play(1);
	game.play(2);
	game.play(3);
	game.play(4);

	// game.play(6);
	// game.play(5);
	// game.play(19);
	// game.play(12);
	// game.play(21);
	// game.play(29);
	// game.play(7);
	// game.play(8);
	// game.play(9);







	// game.play(5);
	// game.play(7);
	// game.play(6);
	// game.play(20);






	std::cout << game << "\nStarting board\n";
	// std::cout << game.board().free_tiles() << '\n'; 
	// for (auto i : game.board().free_tiles().binary_to_vector()) {
		// std::cout << i << '\n';
	// }
	// std::cout << game.ongoing() << '\n';
	auto depth = 3;
	while (game.terminal() == false) {
		// int player_move;
		// std::cin >> player_move;
		// game.play(player_move);
		// std::cout << game << '\n'; 
		
		// std::cout << "Generating moves ...\n";
		// game.generate_to_depth(depth);
		std::cout << "Minmax" << '\n';
		auto move = game.minmax(depth);
		std::cout << "Move: " << move << '\n';
		game.play(move);
		game.clear();
		std::cout << game << '\n'; 
		// break;
	}

	return 0;
}
