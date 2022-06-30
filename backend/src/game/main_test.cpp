#include <vector>
#include <iostream>

#include "board.hpp"
#include "computer.hpp"
#include "game.hpp"


auto main(void) -> int {
	auto board = Board(2);
	board.set(10, 0);
	board.set(3, 1);
	board.set(25, 0);
	board.set(40, 1);
	std::cout << board << '\n';

	auto p1 = board.player_tiles(0);
	auto p2 = board.player_tiles(1);

	std::cout << (p1 ^ p2) << '\n';

	return 0;
}

// // // auto main(void) -> int {
// // // 	auto uids = std::vector<int>{100, 200};
// // // 	auto board = Board(2, uids);

// // // 	while (board.game_status() == Board::state::ONGOING) {
// // // 		std::cout << board << "Player: " << board.whose_turn() << "\tMove: \n\n";
// // // 		std::string coord;
// // // 		std::cin >> coord;
// // // 		board.play_move(coord);
		
// // // 		std::cout << board << '\n';
// // // 		auto computer = Computer(board);
// // // 		Hexagon &hex = board.find_tile(computer.make_random_move());
// // // 		board.play_move(hex);
// // // 	}
// // // 	std::cout << board << "\n";

// // // 	auto result = board.game_status();
// // // 	std::string output;
// // // 	if (result == Board::state::WIN) output = std::string(" won!");
// // // 	if (result == Board::state::LOSS) output = std::string(" lost!");
// // // 	if (result == Board::state::DRAW) output = std::string(" drew!");

// // // 	std::cout << "Player: " << board.whose_turn() << output << '\n';

// // // 	return 0;
// // // }

// // auto main(void) -> int {
// // 	// auto vec = axial::vector{-1,0,1};
// // 	// std::cout << "Index: " << axial::vector::index(vec) << '\n';
// // 	// auto board = Board(2);
// // 	// board.set(30, 1);
// // 	// board.set(15, 0);
// // 	// board.set(45, 1);
// // 	// board.set(41, 0);
// // 	// board.set(4, 1);
// // 	// board.set(38, 1);


// // 	// auto p1 = board.player_tiles(1);
// // 	// auto diagonal = Board::axis(4, axial::vector::us());
// // 	// std::cout << "Three: " << Board::check_n(p1, diagonal, 3) << '\n';


// // 	for (int i = 0; i < 61; i++) {
// // 		auto game = Game(2);
// // 		game.play(i);

// // 		std::cout << game << '\n';
// // 		std::cout << Game::indexToCoord(i) << '\n';
// // 		std::cout << Game::coordToIndex(Game::indexToCoord(i)) << '\n';
// // 	}


// // 	// std::cout << p1 << '\n';
// // 	// std::cout << diagonal << '\n';
// // 	// std::cout << (p1 & diagonal) << '\n';


// // 	// std::cout << board << '\n';	
// // 	// std::cout << board.free_tiles() << '\n';	

// // 	// std::cout << board.free_tiles(30, axial::vector::uqr()) << '\n';

// // 	// for (auto i : board.free_tiles(15, axial::vector::ur()).binary_to_vector()) {
// // 	// 	std::cout << "i: " << i << '\n';
// // 	// }

// // }