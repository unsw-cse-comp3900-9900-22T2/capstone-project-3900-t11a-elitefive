#include <iostream>
#include <vector>

#include "axial.hpp"
#include "hexagon.hpp"

auto convert_axial_to_2d_array(axial::vector const& position, int const& board_radius) -> int;
auto generate_classic_board() -> std::vector<Hexagon>;

int main(void) {
	auto board = generate_classic_board();
	for (auto tile : board) {
		if (!tile.valid()) continue;
		std::cout << tile << '\n';
	}
	return 0;
}

auto generate_classic_board() -> std::vector<Hexagon> {
	auto const board_radius = 4;
	auto boardstate = std::vector<Hexagon>(81, {axial::vector{0,0,0}, -1, Hexagon::EMPTY});
	auto center_tile_index = convert_axial_to_2d_array(axial::vector{0,0,0}, board_radius);
	boardstate[center_tile_index] = Hexagon(axial::vector{0,0,0}, center_tile_index);
	
	for (int radius = 1; radius <= board_radius; ++radius) {
		auto curr_vec = axial::vector::us() * radius;	// South West starting position
		// Go around in circle
		for (auto unit_vec : axial::vector::unit_directions()) {
			for (int i = 0; i < radius; ++i) {
				// std::cout << curr_vec << '\n';
				auto const position = convert_axial_to_2d_array(curr_vec, board_radius);
				boardstate[position] = Hexagon(curr_vec, position); 
				// std::cout << "pos: " << position << '\n';
				curr_vec += unit_vec;
			}
		}
	}

	return boardstate;
}

auto convert_axial_to_2d_array(axial::vector const& position, int const& board_radius) -> int {
	// TODO: Need to check whether the capture is being passed correctly
	auto const board_length = board_radius * 2 + 1;
	auto get_row = [board_radius](axial::vector vec) -> int {
		return vec.q() + board_radius;
	};

	auto get_column = [board_radius](axial::vector vec) -> int {
		return -vec.s() + board_radius;
	};

	return get_row(position) * board_length + get_column(position);
}
