#ifndef YAVALATH_METADATAGEN_HPP
#define YAVALATH_METADATAGEN_HPP

#include "game.hpp"
#include "bitboard.hpp"

class MetaDataGenerator {
	private:
		int nplayers_;
		std::vector<std::string> move_sequence_;
		std::vector<std::vector<std::string>> moves_by_player_;
		std::vector<BitBoard> positions_;
	public:
		MetaDataGenerator(Game game);
		MetaDataGenerator(std::string moves, int players);

		auto num_players() const -> int;
		auto db_snapshot() const -> std::vector<uint64_t>;
		auto moves_by(int player) const -> std::vector<std::string>;

};



#endif