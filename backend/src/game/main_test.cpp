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
	game.play(5);
	game.play(6);
	game.play(7);
	game.play(8);
	game.play(9);
	game.play(10);
	game.play(11);
	game.play(12);
	game.play(13);
	game.play(14);
	game.play(15);
	game.play(16);
	game.play(17);
	game.play(18);
	game.play(19);
	game.play(20);
	game.play(21);
	game.play(22);
	game.play(23);
	game.play(24);
	game.play(25);
	game.play(26);
	game.play(27);
	game.play(28);
	game.play(29);
	game.play(30);
	game.play(31);
	game.play(32);
	game.play(33);
	game.play(34);
	game.play(36);
	game.play(35);
	// ^^ Top half of the board ^^
	game.play(38);
	game.play(37);
	game.play(40);
	game.play(39);
	/* game.play(42); */ game.pass_turn();
	game.play(41);
	game.play(43);
	game.play(44);
	game.play(45);
	game.play(46);
	game.play(47);
	game.play(48);
	
	// game.play(49);
	game.play(57);

	game.play(48);
	game.play(50);
	game.play(51);
	game.play(52);
	game.play(53);
	game.play(54);
	game.play(55);
	game.pass_turn();

	game.play(42);



	std::cout << game << "\nStarting board\n";
	// std::cout << game.board().free_tiles() << '\n'; 
	// for (auto i : game.board().free_tiles().binary_to_vector()) {
		// std::cout << i << '\n';
	// }
	// std::cout << game.ongoing() << '\n';
	auto depth = 1;
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
		break;
	}

	return 0;
}