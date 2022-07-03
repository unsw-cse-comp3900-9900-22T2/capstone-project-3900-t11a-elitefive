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
	game.play(31);
	game.play(15);
	game.play(45);
	game.play(41);
	game.play(51);
	// game.play(32);
	// game.play(0);
	std::cout << game << "\nStarting board\n";

	auto depth = 5;
	while (game.terminal() == false) {
		// int player_move;
		// std::cin >> player_move;
		// game.play(player_move);
		// std::cout << game << '\n'; 
		
		std::cout << "Generating moves ...\n";
		game.generate_to_depth(depth);
		std::cout << "Minmax" << '\n';
		auto move = game.minmax(depth);
		std::cout << "Move: " << move << '\n';
		game.play(move);
		game.clear();
		std::cout << game << '\n'; 
		break;
	}


	// Board b = Board(2);
	// b.set(0,1);
	// b.set(1,0);
	// b.set(2,1);

	// std::cout << b << '\n';
	// std::cout << b.free_tiles() << '\n';
	// std::cout << b.player_tiles(0) << '\n';
	// std::cout << b.opponent_tiles(0) << '\n';

	// auto game = AIGame(2);
	// game.play(31);
	// game.play(15);
	// game.play(45);
	// game.play(41);
	// game.play(51);
	// game.play(32);
	// game.play(0);

	// std::cout << game << '\n';


	
	// auto depth = 3;
	// std::cout << "Generating moves ...\n";
	// game.generate_to_depth(depth);
	// std::cout << "Minmax" << '\n';
	// auto move = game.minmax(depth);
	// std::cout << "Move: " << move << '\n';
	// game.play(move);
	// std::cout << game << '\n';


	// game.generate_all_moves();
	// game.find_best();
	// std::cout << game.heuristic() << '\n';
	// std::cout << game << '\n';

	return 0;
}