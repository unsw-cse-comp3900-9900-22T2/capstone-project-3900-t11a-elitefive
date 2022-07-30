#include <vector>
#include <iostream>

#include "board.hpp"
#include "computer.hpp"
#include "aigame.hpp"
#include "game.hpp"
#include "bitboard.hpp"
#include "metadatagen.hpp"
#include "search.hpp"

#include <unordered_map>
#include <memory.h>

auto generate_test_game() -> AIGame;
auto ai_play_game(AIGame &game) -> void;
auto computer_verse_computer() -> void;


auto main(void) -> int {
	// Search game = Search(2, BitBoard(288230376151711743));
	Search game = Search(2, 0);
	game.play(30);
	game.play(39);
	game.play(31);
	game.play(46);
	// game.play(28);
	// game.play(60);

	// game.play(30);
	// game.play(38);
	// game.play(31);
	// game.play(12);
	std::cout << game << '\n';
	int move = game.minmax();
	game.play(move);
	std::cout << game << '\n';

	// int depth = 3;
	// auto memo = SearchMemo();
	// for (int depth = 1; depth <= 2; ++depth) {
	// 	std::cout << game << '\n';
	// 	std::cout << "Depth: " << depth << '\n';
	// 	std::pair<int, int> res = game.run_minmax(depth, game.whose_turn(), memo, -99999, 99999);
	// 	memo.clear();
	// 	int score = res.first;
	// 	int move = res.second;
	// 	std::cout << "Minmax finished\n";
	// 	std::cout << "Score: " << score << '\n';
	// 	std::cout << "Move: " << move << '\n';
	// 	if (std::abs(score) >= 1000) break;
	// }

	// auto memo = Memo();
	// int score = game.minmax(5, 0);


	// auto game = generate_test_game();
	// ai_play_game(game);
	
	return 0;
}

auto computer_verse_computer() -> void {
	for (int i = 0; i < 10000; ++i) {
		auto game = Game(2);
		auto computer = Computer(game);
		while (game.ongoing()) {
			auto const move = computer.make_random_move(std::as_const(game).board());
			game.play(move);
		}
		std::cout << game << '\n';
	}
}

auto generate_test_game() -> AIGame {
	// auto game = AIGame(2);
	// game.pass_turn();
	// game.play(1); game.pass_turn();
	// game.play(3); game.pass_turn();
	// game.play(4); //game.pass_turn();

	// // game.play(45); game.pass_turn();
	// // game.play(46); game.pass_turn();
	// // game.play(48); game.pass_turn();
	// return game;
	
	auto game = AIGame(2);
	// game.pass_turn();
	game.play(30);
	game.play(38);
	game.play(31);
	game.play(12);
	// game.play(28);

	// game.play(6);
	// game.play(5)
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
	return game;
}

auto ai_play_game(AIGame &game) -> void {
	std::cout << game << "\nStarting board\n";
	auto depth = 2;
	while (game.isTerminal() == false) {
		// int player_move;
		// std::cin >> player_move;
		// game.play(player_move);
		// std::cout << game << '\n'; 
		
		// std::cout << "Generating moves ...\n";
		// game.generate_to_depth(depth);
		std::cout << "Minmax" << '\n';
		auto move = game.minmax(depth, 0);
		std::cout << "Move: " << move << '\n';
		game.play(move);
		game.clear();
		std::cout << game << '\n'; 
		break;
	}
}