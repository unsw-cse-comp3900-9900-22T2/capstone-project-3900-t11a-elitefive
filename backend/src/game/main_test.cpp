#include <vector>
#include <iostream>

#include "board.hpp"
#include "computer.hpp"
#include "aigame.hpp"
#include "game.hpp"
#include "bitboard.hpp"
#include "metadatagen.hpp"

#include <unordered_map>
#include <memory.h>

auto generate_test_game() -> AIGame;
auto ai_play_game(AIGame &game) -> void;
auto computer_verse_computer() -> void;


// struct BoardHashTEST
// {
// 	std::size_t operator()(std::vector<BitBoard> const& key) const
// 		{
// 		BitBoard seed = BitBoard();
// 		for (auto const& i : key) {
// 			seed = seed | i;
// 		}
// 		return seed.value();
// 	}
// };

// struct BoardEqualTEST
// {
// 	bool operator()(std::vector<BitBoard> const& lhs, std::vector<BitBoard> const& rhs) const {	
// 		if (lhs.size() != rhs.size()) return false;
// 		return std::equal(lhs.begin(), lhs.end(), rhs.begin());
// 	}
// };
// 	// auto map = std::unordered_map<std::vector<BitBoard>, std::unique_ptr<AIGame>, BoardHashTEST, BoardEqualTEST> {
// 	// 	// {{3,4}, "There"}
// 	// };

// 	// map.insert({game.board().all_boards(), std::make_unique<AIGame>(game, 3)});

// 	// if(map.find(game.board().all_boards()) != map.end()) {
// 	// 	std::cout <<  *map[game.board().all_boards()];
// 	// }

auto main(void) -> int {
	Game game = Game(2);
	game.play("a1");
	game.play("b1");
	game.play("a2");
	game.play("b2");
	game.play("a4");
	game.play("c3");
	game.play("a3");
	std::cout << game << '\n';

	auto meta = MetaDataGenerator(game);
	for (auto pos : meta.db_snapshot()) {
		std::cout << pos << '\n';
	}
	


	// // auto game = AIGame(2);
	// // game.play(4);
	// // game.play(2);
	// // game.play(1);
	// // game.play(3);

	// // auto memo = Memo();
	// // memo.insert(game, 3);
	// // auto boards = game.board().all_boards();
	// // std::cout << memo.contains(boards) << '\n';
	// // std::cout << memo.find(boards) << '\n';

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
	auto depth = 5;
	while (game.isTerminal() == false) {
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
}