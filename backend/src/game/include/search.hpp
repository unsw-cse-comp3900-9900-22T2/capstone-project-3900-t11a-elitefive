#ifndef YAVALATH_SEARCH_HPP
#define YAVALATH_SEARCH_HPP

#include <string.h>
#include <vector>
#include <algorithm>
#include <random>
#include <memory>
#include <unordered_map>

#include "basegame.hpp"
#include "bitboard.hpp"

class SearchMemo;

class Search : public BaseGame {
    public:
		enum class terminal {
			PLAYER0_WIN, PLAYER0_LOSS,
			PLAYER1_WIN, PLAYER1_LOSS,
			NONE, DRAW
        };
    private:
        terminal reason_;   // Board state reached final position
        int score_;         // Heuristic Score
        int difficulty_;    // 
    public:
        Search(int nplayers, int difficulty, BitBoard potholes = BitBoard()) 
        : BaseGame{nplayers, potholes}
        , reason_{terminal::NONE}
        , score_{0} // Assume even
        , difficulty_{difficulty}
        {}

        Search(Search const& position, int score = 0)
        : BaseGame{position.board(), position.whose_turn(), position.num_moves()}
        , score_{ score }
        , reason_{ position.reason() }
        , difficulty_{position.diff() }
        {}

        ~Search() = default;

        auto score() const -> int { return score_; }
        auto reason() const -> terminal { return reason_; }
        auto diff() const -> int { return difficulty_; }

        auto play(std::string move) -> bool {
	        auto position = BaseGame::coordToIndex(move);
	        return play(position);
        }

        auto play(int index) -> bool {
            auto const curr_player = this->whose_turn();
            board().set(index, curr_player);
            if (won(index, curr_player)) {
                if (curr_player == 0) reason_ = terminal::PLAYER0_WIN;
                if (curr_player == 1) reason_ = terminal::PLAYER1_WIN;

            } else if (loss(index, curr_player)) {
                if (curr_player == 0) reason_ = terminal::PLAYER0_LOSS;
                if (curr_player == 1) reason_ = terminal::PLAYER1_LOSS;
            }
            end_turn();
            return true;
        }

        auto unplay(int index) -> void {
            this->decrease_move();
            this->unpass_turn();
            reason_ = terminal::NONE;
            score_ = 0;		// TODO: Could be a problem

            board().unset(index, this->whose_turn());
        }

        auto end_turn() -> void {
	        increase_move();
	        pass_turn();
        }

        auto minmax(int todepth) -> int;

        auto run_minmax(int depth, int player, SearchMemo &memo, int alpha, int beta) -> std::pair<int, int>;

        // =======================================
        int heuristic();
        // =======================================
        
        int random_eval() {
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> values(-50,50);
            return values(rng);
        }

        int triangle_strat(int const player) {
            // Assume current player is trying to win
            Board const &board = this->board();
            BitBoard const free_spaces = board.free_tiles();
            BitBoard const player_tiles = board.player_tiles(player);
            BitBoard const opponent_tiles = board.opponent_tiles(player);

            std::vector<int> const& tiles = player_tiles.binary_to_vector();
            int triangle_score = 0;
            for (int const& tile : tiles) {
                {
                    // Upright triangle
                    axial::vector lvec = axial::vector(tile) + axial::vector::us() * 3;
                    axial::vector rvec = axial::vector(tile) + axial::vector::usq() * 3;
                    if (lvec.distance() > 4) continue; // Ignore tiles outside the board
                    if (rvec.distance() > 4) continue; // Ignore tiles outside the board
                    if (isSet(player_tiles, lvec) && isSet(player_tiles, rvec)) {
                        int lpoint = axial::vector::index(lvec);
                        // int rpoint = axial::vector::index(rvec);
                        // std::cout << "Found triangle\n";
                        // std::cout << tile << " " << lpoint << " " << rpoint << '\n';
                        triangle_score += 15;

                        // Check straight line potential in triangle
                        triangle_score += straight_line(axial::vector::us(), tile, player_tiles, opponent_tiles);
                        triangle_score += straight_line(axial::vector::usq(), tile, player_tiles, opponent_tiles);
                        triangle_score += straight_line(axial::vector::uq(), lpoint, player_tiles, opponent_tiles);
                        if (triangle_score == 15) triangle_score = 0;
                    }
                }
                {
                    // Upside down triangle
                    axial::vector lvec = axial::vector(tile) + axial::vector::ur() * 3;
                    axial::vector rvec = axial::vector(tile) + axial::vector::uqr() * 3;
                    if (lvec.distance() > 4) continue; // Ignore tiles outside the board
                    if (rvec.distance() > 4) continue; // Ignore tiles outside the board
                    if (isSet(player_tiles, lvec) && isSet(player_tiles, rvec)) {
                        int lpoint = axial::vector::index(lvec);
                        int rpoint = axial::vector::index(rvec);
                        // std::cout << "Found triangle\n";
                        // std::cout << tile << " " << lpoint << " " << rpoint << '\n';
                        // exit(1);
                        triangle_score += 15;

                        // Check straight line potential in triangle
                        triangle_score += straight_line(axial::vector::ur(), tile, player_tiles, opponent_tiles);
                        triangle_score += straight_line(axial::vector::uqr(), tile, player_tiles, opponent_tiles);
                        triangle_score += straight_line(axial::vector::uq(), lpoint, player_tiles, opponent_tiles);
                        if (triangle_score == 15) triangle_score = 0;
                    }
                }
            }
            return triangle_score;
        }

        int strat_simple_line(int const player) {
            // Assume current player is trying to win
            Board const &board = this->board();
            BitBoard const free_spaces = board.free_tiles();
            BitBoard const player_tiles = board.player_tiles(player);
            BitBoard const opponent_tiles = board.opponent_tiles(player);

            auto count_lines = 0;
            auto count_distance = 0;

            std::vector<int> const& tiles = player_tiles.binary_to_vector();
            auto const units = axial::vector::unit_directions();
            for (int const& tile : tiles) {
                axial::vector curr_tile = axial::vector(tile);
                for (axial::vector const& dir : units) {
                    count_lines += straight_line(dir, tile, player_tiles, opponent_tiles);
                }

                // penalise tile location if it's far away
                if (isSet(player_tiles, tile)) {
                    count_distance -= curr_tile.distance();
                }
            }
            return (count_lines/2) + count_distance;
        }

        // Helper functions
        auto isSet(BitBoard const& board, axial::vector const& vec) -> bool {
            auto const tile = axial::vector::index(vec);
            if (board.isSet(tile)) return true;
            return false;
        }

        auto straight_line(axial::vector const& dir, int tile, BitBoard const& player_tiles, BitBoard const& opponent_tiles) -> int {
            int count = 0;
            // Make sure tile is INBOUNDS
            axial::vector const target_vec = dir * 3;
            axial::vector const vec_location = axial::vector(tile) + target_vec;
            // Doesn't land on target
            if (vec_location.distance() > 4) return 0; // Ignore tiles outside the board

            if (!isSet(player_tiles, vec_location)) 		return 0; // You don't have a tile here
            if (isSet(opponent_tiles, (tile + dir))) 		return 0; // Opponent tile blocks
            if (isSet(opponent_tiles, tile + (dir * 2))) 	return 0; // Opponent tile blocks
            
            if (isSet(player_tiles, (tile + dir))) ++count; 	// Weight progress higher
            if (isSet(player_tiles, tile + (dir * 2))) count = count + 20; // Weight progress higher

            ++count; // Potential for a connect 4
            return count;
        }


};


class SearchMemo {
	struct BoardHash {
		std::size_t operator()(std::vector<BitBoard> const& key) const {
			BitBoard seed = BitBoard();
			for (auto const& i : key) {
				seed = seed | i;
			}
			return seed.value();
		}
	};

	struct BoardEqual {
		bool operator()(std::vector<BitBoard> const& lhs, std::vector<BitBoard> const& rhs) const {	
			if (lhs.size() != rhs.size()) return false;
			return std::equal(lhs.begin(), lhs.end(), rhs.begin());
		}
	};
	private:
		std::unordered_map<std::vector<BitBoard>, std::unique_ptr<Search>, BoardHash, BoardEqual> map_;
	public:
        std::unordered_map<std::vector<BitBoard>, std::vector<int>, BoardHash, BoardEqual> p1_order_;
        std::unordered_map<std::vector<BitBoard>, std::vector<int>, BoardHash, BoardEqual> p0_order_;

		SearchMemo() 
		: map_{} 
        , p0_order_{}
        , p1_order_{}
		{}

		auto insert_if_not_stored(Search const& game, int score) -> void {
			if (this->contains(game.board().all_boards())) {
				// ALREADY INSERTED
				return;
			}
			this->insert(game, score);
		}

		auto insert(Search const& game, int score) -> void {
			map_.insert({
				game.board().all_boards(),
				std::make_unique<Search>(game, score)
			});
		}

		auto contains(std::vector<BitBoard> const& boards) const -> bool {
			return (map_.find(boards) != map_.end());
		}

		// NOTE: Need to check if it contains first
		auto find(std::vector<BitBoard> const& boards) -> Search *{
			return map_.find(boards)->second.get();
		}

        auto clear() -> void {
            map_.clear();
        }

        // ------------
        // For ordering
        // -------------
        auto has_order(std::vector<BitBoard> const& boards, int player) -> bool {
			if (player == 0) return (p0_order_.find(boards) != p0_order_.end());
			if (player == 1) return (p1_order_.find(boards) != p1_order_.end());
            std::cout << "Player doesn't exist\n";
            exit(1);
        }

        auto get_order(std::vector<BitBoard> const& boards, int player) -> std::vector<int> {
			// std::cout << "Getting ORDER\n";
            if (player == 0) return p0_order_.find(boards)->second;
			if (player == 1) return p1_order_.find(boards)->second;
        }

        auto store_order(std::vector<BitBoard> const& boards, std::vector<int> const order, int player) -> void {
			// std::cout << "store ORDER\n\t";
            // for (auto move : order) {
            //     std::cout << move << ' ';
            // }
            // std::cout << '\n';
            if (player == 0) { p0_order_.insert({ boards, order }); return; }
			if (player == 1) { p1_order_.insert({ boards, order }); return; }
            std::cout << "Couldn't insert order\n";
            exit(1);
        }
};

#endif
