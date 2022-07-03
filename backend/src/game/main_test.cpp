#include <vector>
#include <iostream>

#include "board.hpp"
#include "computer.hpp"
#include "aigame.hpp"
#include "game.hpp"
#include "bitboard.hpp"


auto main(void) -> int {

	auto game = AIGame(2);
	auto depth = 3;
	while (game.terminal() == false) {
		int player_move;
		std::cin >> player_move;
		game.play(player_move);
		std::cout << game << '\n'; 
		
		std::cout << "Generating moves ...\n";
		game.generate_to_depth(depth);
		std::cout << "Minmax" << '\n';
		auto move = game.minmax(depth);
		std::cout << "Move: " << move << '\n';
		game.play(move);
		game.clear();
		std::cout << game << '\n'; 

	}



	// game.play(1);
	// game.play(14);
	// game.play(3);
	// game.play(11);
	// game.play(4);
	// game.play(13);
	// game.play(5);

	// game.play(30);
	// game.play(15);
	// game.play(45);
	// game.play(41);
	// game.play(51);
	// game.play(32);
	// game.play(0);



	
	// auto depth = 3;
	// std::cout << game << '\n';
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