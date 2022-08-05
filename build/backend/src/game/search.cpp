#include <string.h>
#include <vector>
#include <assert.h> // TODO: DELETE
#include <algorithm>
#include "search.hpp"
#include "basegame.hpp"


auto Search::minmax(int todepth) -> int {
	auto memo = SearchMemo();
    
    int max_depth = todepth;
    if (max_depth > 3) max_depth = 3;
    
    // Since easy difficulty is randomness with a lookup, limit the depth of minmax agressively
    if (todepth > 3) {
        if (diff() == 0) {
            if (this->num_moves() >= 20 && this->board().num_spaces() - this->num_moves() <= 40) {
                std::cout << "Letting random be even more random\n";
                max_depth = 4;
                if (this->num_moves() >= 35) max_depth = 5;
            }
        }
        if (diff() == 2 && this->num_moves() >= 10) {
            max_depth = 4;
        }
    }
    
    int move = 0;
    for (int depth = 1; depth <= max_depth; ++depth) {
        std::pair<int, int> res = run_minmax(depth, whose_turn(), memo, -99999, 99999);
        memo.clear();
        int score = res.first;
        move = res.second;
        if (std::abs(score) >= 1000) break;
    }

    return move;
}

int Search::heuristic() {
    if (diff() == 0) {
        return random_eval();
    }
    if (diff() == 1) {
        int triangle = triangle_strat(0) - triangle_strat(1);
        int line = strat_simple_line(0) - strat_simple_line(1);
        return triangle + line;
    }
    if (diff() == 2) {
        int triangle = triangle_strat(0) - triangle_strat(1);
        int line = strat_simple_line(0) - strat_simple_line(1);
        return triangle + line;
    }
    std::cout << "Difficulty no implemented yet\n";
    exit(1);
}

// std::pair<int, int>
//      || int: score
//      || int: move
auto Search::run_minmax(int depth, int player, SearchMemo &memo, int alpha, int beta) -> std::pair<int, int> /*score of state*/ {
    if (this->reason_ == Search::terminal::PLAYER0_WIN) {
        // std::cout << "At terminal state p0w\n";
        return  { 1000 + depth, -1};
    }
    if (this->reason_ == Search::terminal::PLAYER1_WIN) {
        // std::cout << "At terminal state p1w\n";
        return  {-1000 - depth, -1};
    }
    if (this->reason_ == Search::terminal::PLAYER0_LOSS) {
        // std::cout << "At terminal state p0l\n";
        return {-1000 - depth, -1};
    }
    if (this->reason_ == Search::terminal::PLAYER1_LOSS) {
        // std::cout << "At terminal state p1l\n";
        return { 1000 + depth, -1};
    }
    if (this->board().num_spaces() == this->num_moves()) {
        return {0, -1}; // DRAW
    }

    if (depth == 0) { return {heuristic(), -1}; }

    if (player == 0) {
        Search search = Search(*this);  // Search from this state
        
        // Assume that there are no good moves
        int bestmove = 0;
        int maxEval = -99999;

        // Search each position in this state
        auto const tiles = this->board().all_boards();
        std::vector<int> ordering;
        if (memo.has_order(tiles, player)) ordering = memo.get_order(tiles, player);
        else {
            // First time we have had this position. Mix it up a bit for each game
            ordering = board().free_tiles().binary_to_vector();
            std::random_device dev;
            std::mt19937 rng(dev());
            std::shuffle(ordering.begin(), ordering.end(), rng);
        }
        
        std::vector<std::pair<int, int>> candidate_order = {};
        int index = 0;
        for (auto const& move : ordering) {
            ++index;
            // std::cout << "Considering move: " << move  << '\n';
            search.play(move);
            int search_evaluation;
            if (!memo.contains(search.board().all_boards())) {
                // Compute state
                std::pair<int, int> result = search.run_minmax(depth - 1, 1, memo, alpha, beta);
                int score = result.first;
                memo.insert_if_not_stored(search, score);
                
                search_evaluation = score;
            }
            else {
                // We have been to this state earlier in the search. Reuse result.
                Search *precomputed_game = memo.find(search.board().all_boards());
                int precomputed_score = precomputed_game->score();
                
                search_evaluation = precomputed_score;
            }

            candidate_order.push_back({search_evaluation, move});
            // Decide if this is a better move
            if (search_evaluation > maxEval) {
                maxEval = search_evaluation;
                bestmove = move;
            }
            if (search_evaluation >= beta) break; // Give up looking
            alpha = std::max(alpha, search_evaluation);

            search.unplay(move);
        }

        // ================ For later look ups ===================
        // Sort higher scoring positions first
		for (; index < ordering.size(); index++) {
            candidate_order.push_back({-10000, ordering[index]});   // De-priotize states we didn't look at
        }
        std::stable_sort(candidate_order.begin(), candidate_order.end(), [](std::pair<int, int> const& a, std::pair<int, int> const& b) {
            return a.first > a.second;
		});
        // Get the order of the moves
        std::vector<int> search_order = {};
        std::transform(candidate_order.begin(), candidate_order.end(), std::back_inserter(search_order), [](const std::pair<int, int>& d){return d.second;});
        memo.p0_order_[tiles] = search_order;
        // ================ For later look ups ===================

        // std::cout << "Best move: " << bestmove << " with score " << maxEval << '\n';
        return {maxEval, bestmove};
    }

    if (player == 1) {
        Search search = Search(*this);  // Search from this state
        
        // Assume that there are no good moves
        int worstmove = 0;
        int minEval = 99999;
        
        // === For move ordering later ===
        std::vector<std::pair<int, int>> candidate_order = {};
        int index = 0;
        // === For move ordering later ===

        // Search each position in this state based on promising move order
        auto const tiles = this->board().all_boards();
        std::vector<int> ordering;
        if (memo.has_order(tiles, player)) ordering = memo.get_order(tiles, player);
        else {
            // First time we have had this position. Mix it up a bit for each game
            ordering = board().free_tiles().binary_to_vector();
            std::random_device dev;
            std::mt19937 rng(dev());
            std::shuffle(ordering.begin(), ordering.end(), rng);
        }
        for (auto const& move : ordering) {
            ++index;
            search.play(move);
            int search_evaluation;
            if (!memo.contains(search.board().all_boards())) {
                // Compute state
                std::pair<int, int> result = search.run_minmax(depth - 1, 0, memo, alpha, beta);
                int score = result.first;
                memo.insert_if_not_stored(search, score);

                search_evaluation = score;
            }
            else {
                // We have been to this state earlier in the search. Reuse result.
                Search *precomputed_game = memo.find(search.board().all_boards());
                int precomputed_score = precomputed_game->score();
                
                search_evaluation = precomputed_score;
            }
            candidate_order.push_back({search_evaluation, move});

            // Decide if this is a better move
            if (search_evaluation < minEval) {
                minEval = search_evaluation;
                worstmove = move;
            }
            if (search_evaluation <= alpha) break;
            beta = std::min(beta, search_evaluation);

            search.unplay(move);
        }

        // ================ Sort ordering for later look ups ===================
		// Push back states that we didn't look at
        for (; index < ordering.size(); index++) {
            candidate_order.push_back({10000, ordering[index]});    // De-priotize states we didn't look at
        }
        // Sort lower scoring positions first
        std::stable_sort(candidate_order.begin(), candidate_order.end(), [](std::pair<int, int> const& a, std::pair<int, int> const& b) {
            return a.first < a.second;
		});
        // Get the order of the moves
        std::vector<int> search_order = {};
        std::transform(candidate_order.begin(), candidate_order.end(), std::back_inserter(search_order), [](const std::pair<int, int>& d){return d.second;});   
        memo.p1_order_[tiles] = search_order;
        // ================ For later look ups ===================

        return {minEval, worstmove};
    }


    return {-1, -1};
}