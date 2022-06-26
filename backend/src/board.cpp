#include <iostream>
#include <vector>
#include <assert.h>
#include <string>

#include "axial.hpp"
#include "hexagon.hpp"
auto convert_axial_to_2d_array(axial::vector const& position, int const& board_radius) -> int;
auto generate_classic_board() -> std::vector<Hexagon>;
auto draw_board(std::vector<Hexagon> tiles) -> void;

// Return the tile location
// -1 indicates that the move is not valid
auto move_converter(std::string move) -> int {
	std::cout << move << '\n';
	// TODO: Convert into a valid move to pass to the board
}

int main(void) {
	auto board = generate_classic_board();
	auto player = 0;
	while (true) {
		draw_board(board);
		std::string move;
		std::cout << "Player Turn: " << player << " Move: ";
		std::cin >> move;
		std::cout << move;
		auto index = move_converter(move);
		// board[move].used_ = player;
		player = (player + 1) % 2;
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

auto draw_board(std::vector<Hexagon> tiles) -> void {
	auto const board_width = 9;
	
	auto tile_lookup = [tiles, board_width](int row, int col) {
		auto position = row * board_width + col;
		assert(tiles[position].valid());
		if (tiles[position].used_ == 0) return 'x';
		if (tiles[position].used_ == 1) return 'o';
		return '-';
	};

	for (int row = -4; row <= 4 ; ++row) {
		auto const spacing = (row < 0) ? -row : row;
		for (int i = 0; i < spacing; ++i) {
			std::cout << " ";
		}
		for (int i = 0; i < board_width - spacing; ++i) {
			auto const col = (row < 0) ? 0 : row;
			auto symbol = tile_lookup(row + 4, col + i);
			std::cout << symbol << " ";
		}
		std::cout << '\n';
	}
}
