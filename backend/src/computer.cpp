#include <vector>
#include <algorithm>
#include <random>

#include "computer.hpp"
#include "board.hpp"

Computer::Computer(Board board)
: board_{board}
{}

auto Computer::make_random_move() -> Hexagon& {
	auto const selections = board_.view_available_tiles();
	
	auto random_selection = std::vector<Hexagon>{};
	std::sample(
		selections.begin(),
		selections.end(),
		std::back_inserter(random_selection),
		1,
		std::mt19937{std::random_device{}()}
	);
	// std::cout << random_selection.front() << '\n';
	// TODO: This is broken Broken
	return board_.find_tile(random_selection.front());
}
