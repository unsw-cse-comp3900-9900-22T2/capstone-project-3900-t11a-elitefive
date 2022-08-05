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
#include <map>
#include <set>
#include <fstream>


auto generate_test_game() -> AIGame;
auto ai_play_game(AIGame &game) -> void;
auto computer_verse_computer() -> void;

auto random_num(int bound) -> int {
    auto seed = std::chrono::steady_clock::now().time_since_epoch().count();
    auto e = std::default_random_engine(seed);
    auto distribution = std::uniform_int_distribution<int>(0, bound - 1);
    auto random = distribution(e);
    return random;
}

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
	std::ofstream ofile ("output.txt");
	// Random
	srand ( time(NULL) );
	// Output strings.
	std::vector<std::string> o_users = {};
	std::vector<std::string> o_matches = {};
	std::vector<std::string> o_outcomes = {};
	std::vector<std::string> o_snapshots = {};
	// Num users & matches.
	auto n_users = 30;
	auto n_matches = 300;
	// Users ELO map.
	auto elos = std::map<int, std::map<std::string, int>>{};

	// ----- USERS -----
	// Generating users.
	std::vector<std::string> names = {
		"Doormat", "Upscale", "Dora", "Dingus", "Sonar", "Weightlifter", "Solar", "Vimbro", "Virgo", "Cat", "Dog", "God", "Damo427", "Harry",
		"Guy", "Flushed", "Blushed", "Enclosed", "Superimposed", "TuffGrass", "TheDevGuy", "Latemorning", "Cramp", "Sunrise_tomorrow", "HelloSky", 
		"Pseudonym", "Forelimb", "Halo", "Portal", "Undergrown", "Plant", "Houseman"
	};
	for (int i = 11; i < 11 + n_users; i++) {

		o_users.push_back("('Tester-" + names[i - 11] + "-" + std::to_string(i) + "', 'user" + std::to_string(i) + "@email.com', '619227d5cf63bffd286a6529f58fb3e679169230eb7b0151871b8f6583f24bc6')");
		elos.insert({i, {
			{"CLASSIC", 1000},
			{"TRIPLES", 1000},
			{"POTHOLES", 1000}
		}});
	}

	// ----- MATCHES -----
	// Generating matches.
	for (int i = 0; i < n_matches; i++) {
		auto matchID = 1 + i;
		// Select random gamemode
		int gamenum = select_random({0, 0, 0, 0, 1, 2, 2});
		// int gamenum = 1;
		std::string gamemode = "CLASSIC";
		auto potholes = BitBoard();
		if (gamenum == 1) gamemode = "TRIPLES";
		if (gamenum == 2) { gamemode = "POTHOLES"; potholes = random_potholes(); }

		Game game = make_game(gamenum, potholes);

		if (gamemode != "TRIPLES") {
			Search ai1 = make_ai(gamenum, select_random({0,0,1,2}), potholes);
			Search ai2 = make_ai(gamenum, select_random({0,0,1,2}), potholes);

			while(game.ongoing()) {
				Search &ai = (game.whose_turn() == 0) ? ai1 : ai2;
				int move = ai.minmax(3);
				ai1.play(move);
				ai2.play(move);
				game.play(move);
				// std::cout << game << '\n';
			}
		} else {
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
				}
				if (game.which_player_won() == -1) {
					game = make_game(gamenum, potholes);
					continue;
				}
				break;
			}
		}
		// Debug:
		std::cout << std::to_string(matchID) + "/" + std::to_string(n_matches)
			+ ": Completed " + gamemode + " match!"
			+ " Winner: " + std::to_string(game.which_player_won()) << std::endl;
		
		// Get players
		auto size = (gamemode == "TRIPLES") ? 3 : 2;
		std::set<int> players_set = {};
		while (players_set.size() != size) {
			auto n = 11 + random_num(n_users);
			players_set.insert(n);
		}
		auto players = std::vector<int>(players_set.begin(), players_set.end());
		// Populate Outputs
		// Matches...
		std::string potholes_str = game.list_potholes_string();
		std::string svg = create_svg(game);
		std::string ranked = (select_random({0, 1, 1, 1, 1})) ? "true" : "false";
		o_matches.push_back("('" + gamemode + "', " + ranked + ", '" + potholes_str
			+ "', '" + game.move_sequence() + "', '" + svg + "')");
		// Outcomes...
		for (int i = 0; i < players.size(); i++) {
			auto uid = players.at(i);
			auto elo = elos.at(uid).at(gamemode);
			auto new_elo = 1000;
			if (game.which_player_won() == -1) {
				if (ranked == "true") {
					new_elo = elo;
				}
				o_outcomes.push_back("(" + std::to_string(uid) + ", " + std::to_string(matchID) +
					", " + std::to_string(new_elo) + ", 'DRAW')");
			} else if (game.which_player_won() == i) {
				if (ranked == "true") {
					new_elo = elo + 30;
					elos.at(uid).at(gamemode) = new_elo;
				}
				o_outcomes.push_back("(" + std::to_string(uid) + ", " + std::to_string(matchID) +
					", " + std::to_string(new_elo) + ", 'WIN')");
			} else {
				if (ranked == "true") {
					new_elo = elo - 30;
					elos.at(uid).at(gamemode) = new_elo;
				}
				o_outcomes.push_back("(" + std::to_string(uid) + ", " + std::to_string(matchID) +
					", " + std::to_string(new_elo) + ", 'LOSS')");
			}
		}
		// Snapshots...
		auto gen = MetaDataGenerator(game);
		auto snapshots = gen.db_snapshot();
		for (int i = 0; i < snapshots.size(); i++) {
			o_snapshots.push_back("(" + std::to_string(matchID) + ", " +
				std::to_string(i + 1) + ", " + std::to_string(snapshots.at(i)) + ")");
		}
	}
	// Print Outputs
	if (ofile.is_open()) {
		ofile << "-- Extra Demo Data" << std::endl;
		ofile << std::endl;

		// Users...
		ofile << "-- Insert Users." << std::endl;
		ofile << "insert into users(username, email, password_hash) values " << std::endl;
		for (auto const &u : o_users) {
			ofile << u << (&u == &o_users.back() ? ";" : ", ") << std::endl;
		}
		ofile << std::endl;
		// Matches...
		ofile << "-- Insert Matches." << std::endl;
		ofile << "insert into matches(game, ranked, potholes, replay, svg_data) values " << std::endl;
		for (auto const &u : o_matches) {
			ofile << u << (&u == &o_matches.back() ? ";" : ", ") << std::endl;
		}
		ofile << std::endl;
		// Outcomes...
		ofile << "-- Insert Outcomes." << std::endl;
		ofile << "insert into outcomes values " << std::endl;
		for (auto const &u : o_outcomes) {
			ofile << u << (&u == &o_outcomes.back() ? ";" : ", ") << std::endl;
		}
		ofile << std::endl;
		// Snapshots...
		ofile << "-- Insert Snapshots." << std::endl;
		ofile << "insert into snapshots(match, move_num, boardstate) values " << std::endl;
		for (auto const &u : o_snapshots) {
			ofile << u << (&u == &o_snapshots.back() ? ";" : ", ") << std::endl;
		}
		ofile << std::endl;
	}
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
	auto game = AIGame(2);
	game.play(30);
	game.play(38);
	game.play(31);
	game.play(12);

	return game;
}

auto ai_play_game(AIGame &game) -> void {
	std::cout << game << "\nStarting board\n";
	auto depth = 2;
	while (game.isTerminal() == false) {

		std::cout << "Minmax" << '\n';
		auto move = game.minmax(depth, 0);
		std::cout << "Move: " << move << '\n';
		game.play(move);
		game.clear();
		std::cout << game << '\n'; 
		break;
	}
}