#include <vector>
#include <iostream>

#include "board.hpp"
#include "computer.hpp"

// auto main(void) -> int {
// 	auto uids = std::vector<int>{100, 200};
// 	auto board = Board(2, uids);

// 	while (board.game_status() == Board::state::ONGOING) {
// 		std::cout << board << "Player: " << board.whose_turn() << "\tMove: \n\n";
// 		std::string coord;
// 		std::cin >> coord;
// 		board.play_move(coord);
		
// 		std::cout << board << '\n';
// 		auto computer = Computer(board);
// 		Hexagon &hex = board.find_tile(computer.make_random_move());
// 		board.play_move(hex);
// 	}
// 	std::cout << board << "\n";

// 	auto result = board.game_status();
// 	std::string output;
// 	if (result == Board::state::WIN) output = std::string(" won!");
// 	if (result == Board::state::LOSS) output = std::string(" lost!");
// 	if (result == Board::state::DRAW) output = std::string(" drew!");

// 	std::cout << "Player: " << board.whose_turn() << output << '\n';

// 	return 0;
// }

auto main(void) -> int {
	auto vec = axial::vector{-4,0,4};
	std::cout << axial::vector::index(vec) << '\n';
	auto board = Board(2, uids);
	
}