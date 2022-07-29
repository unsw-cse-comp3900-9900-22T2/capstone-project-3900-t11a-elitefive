#include "metadatagen.hpp"
#include "game.hpp"

auto construct_game(std::string moves, int players) -> Game;

MetaDataGenerator::MetaDataGenerator(Game game)
: nplayers_{game.num_players()}
, move_sequence_{game.all_moves()}
, moves_by_player_(game.num_players())
, positions_{}
{
	// // Generate moves per player
	// auto player = 0;
	// for (auto const& move : move_sequence_) {
	// 	moves_by_player_[player].push_back(move);
	// 	player = (player + 1) % nplayers_;
	// }

	// Generate all unique position
	auto simgame = Game(nplayers_);
	auto const nplayers = simgame.num_players();
	for (auto const& move: move_sequence_) {
		auto player = simgame.whose_turn();
		moves_by_player_[player].push_back(move); // Generating moves by player
		
		auto result = simgame.play(move);
		auto board = simgame.board().player_tiles(player);
		positions_.push_back(board);

		if (nplayers == 3 && simgame.num_players_in() == 3 && simgame.status() == Game::state::LOSS) {
			simgame.set_player_out(simgame.whose_turn());	
			simgame.continue_game();
			simgame.pass_turn();
		}
	}
}

MetaDataGenerator::MetaDataGenerator(std::string moves, int players) : MetaDataGenerator(construct_game(moves, players)) {}

auto construct_game(std::string moves, int players) -> Game {
	auto game = Game(players);
	for(int i = 0; i < moves.size(); i = i+2) {
		std::string move = moves.substr(i, 2);
		game.play(move);
		if (players == 3 && game.num_players_in() == 3 && game.status() == Game::state::LOSS) {
			game.set_player_out(game.whose_turn());	
			game.continue_game();
			game.pass_turn();
		}
	}
	return game;
}

auto MetaDataGenerator::moves_by(int player) const -> std::vector<std::string> {
	return moves_by_player_[player];
}

auto MetaDataGenerator::num_players() const -> int {
	return nplayers_;
}

auto MetaDataGenerator::db_snapshot() const -> std::vector<uint64_t> {
	auto positions = std::vector<uint64_t>{};
	for (auto const& board : positions_) {
		positions.push_back(board.value());
	}
	return positions;
}
