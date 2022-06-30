#include <vector>
#include <algorithm>
#include <random>

#include "computer.hpp"
#include "game.hpp"
#include "board.hpp"
#include "basegame.hpp"

Computer::Computer(Game game)
: game_{game}
{}

auto Computer::make_random_move() -> int {
	
	auto const selections = game_.board().free_tiles().binary_to_vector();
	auto random_selection = std::vector<int>{};
	std::sample(
		selections.begin(),
		selections.end(),
		std::back_inserter(random_selection),
		1,
		std::mt19937{std::random_device{}()}
	);
	// std::cout << "Random selection: " << random_selection.front() << '\n';
	return random_selection.front();
}
