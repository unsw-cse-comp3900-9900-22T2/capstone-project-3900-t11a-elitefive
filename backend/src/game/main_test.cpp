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
#include <random>
#include <chrono>


auto generate_test_game() -> AIGame;
auto ai_play_game(AIGame &game) -> void;
auto computer_verse_computer() -> void;


auto select_random_player(std::vector<int> choices) -> int {
	std::vector<int> out = {};
	std::sample(
        choices.begin(),
        choices.end(),
        std::back_inserter(out),
        choices.size(),
        std::mt19937{std::random_device{}()}
    );
	return out.front();
}

auto create_svg(Game const& game) -> std::string {
	auto svg = std::vector<int>(61, 0);
	// Update svg_data for players.
	auto i = 1;
	for (auto const &board : game.board().all_boards()) {
		auto v = board.binary_to_vector();
		for (auto const &t : v) {
			svg.at(t) = i;
		}
		i++;
	}
	// Update svg_data for potholes.
	auto potholes_vec = game.board().potholes().binary_to_vector();
	for (auto const &t : potholes_vec) {
		svg.at(t) = 4;
	}
	// Create the svg_data string.
	auto svg_data = std::string();
	for (auto const &t : svg) {
		svg_data.append(std::to_string(t));
	}
	return svg_data;
}

auto output_match(std::string gamemode, bool ranked, std::string potholes, std::string replay, std::string svg_data) -> void {
	
	std::string ranked_str = ((ranked) ? "true" : "false");
	std::cout << "('" << gamemode << "', " << ranked_str << ", '" << potholes << "', '" <<  replay << "', '" << svg_data << "'),\n";
}

auto random_potholes() -> std::bitset<64> {
	// Random seed
	auto now = std::chrono::steady_clock::now().time_since_epoch().count();
	auto e1 = std::default_random_engine(now);
	auto d1 = std::uniform_int_distribution<int>(6, 13);
	auto r1 = d1(e1);

	auto potholes = std::vector<int>(61, 0);
	for (int i = 1; i <= r1; i++) {
		auto e2 = std::default_random_engine(now * i);
		auto d2 = std::uniform_int_distribution<int>(0, 60);
		auto r2 = d2(e2);
		if (r2 == 30) continue; // Never get rid of the center tile because BOT3 assumes it's there lol
		potholes.at(r2) = 1;
	}

	auto pots = std::string();
	for (auto const &p : potholes) {
		pots.append(std::to_string(p));
	}
	return std::bitset<64>(pots);
}

auto make_game(int gamemode, BitBoard potholes) -> Game {
	if (gamemode == 0) { return Game(2); }
	if (gamemode == 1) { return Game(3); }
	if (gamemode == 2) { return Game(2, potholes); }
}

auto make_ai(int gamemode, int difficulty, BitBoard potholes) -> Search {
	if (gamemode == 0) { return Search(2, difficulty); }
	// if (gamemode == 1) { return Search(2, difficulty); }
	if (gamemode == 2) { return Search(2, difficulty, potholes); }
}

auto select_random(std::vector<int> nums) {
	int selection = rand() % nums.size();
	return nums[selection];
}

auto main(void) -> int {
	srand ( time(NULL) );
	for (int i = 0; i < 100; i++) {
		// Select random gamemode
		int gamenum = select_random({0, 0, 0, 0, 1, 2, 2});
		// int gamenum = 1;
		std::string gamemode = "CLASSIC";
		auto potholes = BitBoard();
		if (gamenum == 1) gamemode = "TRIPLES";
		if (gamenum == 2) { gamemode = "POTHOLES"; potholes = random_potholes(); }

		Game game = make_game(gamenum, potholes);

		if (gamemode != "TRIPLES") {
			Search ai1 = make_ai(gamenum, 1, potholes);
			Search ai2 = make_ai(gamenum, 2, potholes);

			while(game.ongoing()) {
				Search &ai = (game.whose_turn() == 0) ? ai1 : ai2;
				int move = ai.minmax(3);
				ai1.play(move);
				ai2.play(move);
				game.play(move);
				// std::cout << game << '\n';
			}
		}
		else {
			while(true) {
				int difficulty = 2;
				while(game.status() == Game::state::ONGOING) {
					int turn = game.whose_turn();
					std::vector<int> targets = {2, 2, 2, 1};
					if (turn == 1) targets = {0, 0, 0, 2};
					if (turn == 2) targets = {1, 1, 1, 0};
					
					Search player = Search(game, select_random(targets), difficulty);
					int move = player.minmax(2);
					game.play(move);
					// std::cout << game << '\n';
				}
				if (game.which_player_won() == -1) {
					game = make_game(gamenum, potholes);
					continue;
				}
				break;
			}
		}

		std::cout << "Winner " << game.which_player_won() << " $$$";
		std::string potholes_str = game.list_potholes_string();
		std::string svg = create_svg(game);
		bool ranked = (select_random({0, 1, 1, 1, 1})) ? true : false;
		output_match(gamemode, ranked, potholes_str, game.move_sequence(), svg);
		// std::cout << game << '\n';
		// break;
	}

	// while (game.ongoing()) {
	// 	int move;
	// 	std::cin >> move;
	// 	game.play(move);
	// 	std::cout << game << "\n";
	// 	if (game.ongoing() == false) break;
	// 	Search p1 = Search(game, select_random_player({0, 2}), 1);
	// 	game.play(p1.minmax(4));
	// 	std::cout << game << "\n";
	// 	if (game.ongoing() == false) break;
	// 	Search p2 = Search(game, 1, 1);
	// 	game.play(p2.minmax(4));
	// 	std::cout << game << "\n";
	// }

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