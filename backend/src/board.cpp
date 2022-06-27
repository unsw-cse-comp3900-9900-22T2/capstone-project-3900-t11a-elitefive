#include <iostream>
#include <vector>
#include <assert.h>
#include <string>

#include "axial.hpp"
#include "hexagon.hpp"

auto convert_axial_to_2d_array(axial::vector const& position, int const& board_radius) -> int;
auto generate_classic_board() -> std::vector<Hexagon>;
auto draw_board(std::vector<Hexagon> tiles) -> void;
auto check_outcome(std::vector<Hexagon> tiles, int move, int player) -> int;
auto is_win(std::vector<Hexagon> tiles, int move, int player) -> bool;
auto is_loss(std::vector<Hexagon> tiles, int move, int player) -> bool;
auto is_connected(std::vector<Hexagon> tiles, int move, int player, int n) -> bool;
auto check_connected_n(std::vector<Hexagon> tiles, int player, int n, axial::vector vec, axial::vector dir, bool pos, int &connected) -> bool;
auto coord_to_index(std::string s) -> int;
auto gameloop() -> void;

auto main(void) -> int {
	gameloop();
	return 0;
}

auto gameloop() -> void {
	auto board = generate_classic_board();
	auto player = 0;
	bool ongoing = true;
	auto moves = 0;
	while (ongoing && moves < 61) {
		draw_board(board);
		std::string coord;
		std::cout << "Player Turn: " << player << " Move: \n";
		std::cin >> coord;
		auto move = coord_to_index(coord);
		board[move].used_ = player;
		int o = check_outcome(board, move, player);
		if (o == 0) {
			player = (player + 1) % 2;
		} else {
			ongoing = false;
			draw_board(board);
			std::cout << "Player " << player << " " << ((o > 0) ? "Wins" : "Loses") << "!" << std::endl;;
		}
		moves++;
	}
	std::cout << "Draw\n";
	return;
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

auto convert_axial_to_2d_array(axial::vector const& position, int const& board_radius = 4) -> int {
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

auto check_outcome(std::vector<Hexagon> tiles, int move, int player) -> int {
	// Outcomes - 0: no outcome, 1: win, -1: loss
	if (is_win(tiles, move, player)) {
		return 1;
	} else if (is_loss(tiles, move, player)) {
		return -1;
	}
	return 0;
}

auto is_win(std::vector<Hexagon> tiles, int move, int player) -> bool {
	return is_connected(tiles, move, player, 4);
}

auto is_loss(std::vector<Hexagon> tiles, int move, int player) -> bool {
	return is_connected(tiles, move, player, 3);
}

auto is_connected(std::vector<Hexagon> tiles, int move, int player, int n) -> bool {
	auto vec = tiles[move].getPosVec();
	// For each axial direction
	auto const dirs = axial::vector::basis_vectors();
	for (auto const& dir : dirs) {
		int connected = 1;
		if (check_connected_n(tiles, player, n, vec, dir, true, connected)) {
			return true;
		}
		if (check_connected_n(tiles, player, n, vec, dir, false, connected)) {
			return true;
		}
	}
	return false;
}

auto check_connected_n(std::vector<Hexagon> tiles, int player, int n, axial::vector vec, axial::vector dir, bool pos, int &connected) -> bool {
	for (int i = 1; i < n; ++i) {
		auto v = vec + dir * (pos ? 1 : -1) * i;
		auto index = convert_axial_to_2d_array(v);
		if (index < 0 || index >= 81 || tiles[index].used_ != player) {
			return false;
		} else {
			connected++;
			if (connected == n) {
				return true;
			}
		}
	}
}

auto coord_to_index(std::string s) -> int {
	int letter = s[0];
	if (letter >= 'A' && letter <= 'I') {
		int row = letter - 'A';
		int column = stoi(s.substr(1)) - 1;
		int index = (row <= 4 ? (row * 9) : (row - 4) * 10 + 36) + column;
		return index;
	}
	return -1;
}