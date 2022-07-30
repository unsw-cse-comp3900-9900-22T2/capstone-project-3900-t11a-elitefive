#include <string.h>
#include <vector>
#include <assert.h> // TODO: DELETE
#include <algorithm>
#include "search.hpp"
#include "basegame.hpp"

auto simple_state(Search state) -> std::string {
    std::string out = "";
    std::vector<int> moves = {58, 59, 60};
    for (auto move : moves) {
        int player = state.board().which_player_at(move);
        if (player == 0) out += "1";
        else if (player == 1) out += "2";
        else out += "0";
    }
    return out;
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
        // TODO: NEED TO CHECK IF WON FIRST
        // this->reason_ = Search::terminal::DRAW;
        // this->score_ = 0;
        // std::string output = simple_state(*this);
        // return {atoi(output.c_str()), -1};
        return {0, -1}; // DRAW
    }

    if (depth == 0) {
        // std::cout << "Base case\n";
        // exit(1);
        // return {0, -1};
        int p0 = this->heuristic(0);
        int p1 = this->heuristic(1);
        return {p0 - p1, -1}; // TODO: FIX
    }

    if (player == 0) {
        Search search = Search(*this);  // Search from this state
        
        // Assume that there are no good moves
        int bestmove = 0;
        int maxEval = -99999;

        // std::cout << "Searching at depth: " << depth << "\n"; 
        // Search each position in this state
        auto tiles = board().all_boards();
        std::vector<int> tmp = board().free_tiles().binary_to_vector();
        memo.store_order(search, tmp, 0);
        std::cout << "stored\n";
        std::vector<int> ordering = (memo.contains(tiles)) ? memo.get_order(tiles, 0) : board().free_tiles().binary_to_vector();
        for (auto const& move : ordering) {
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
            
            // Decide if this is a better move
            if (search_evaluation > maxEval) {
                maxEval = search_evaluation;
                bestmove = move;
            }
            if (search_evaluation >= beta) break; // Give up looking
            alpha = std::max(alpha, search_evaluation);

            search.unplay(move);
        }

        // std::cout << "Best move: " << bestmove << " with score " << maxEval << '\n';
        return {maxEval, bestmove};
    }

    if (player == 1) {
        Search search = Search(*this);  // Search from this state
        
        // Assume that there are no good moves
        int worstmove = 0;
        int minEval = 99999;

        // std::cout << "Searching at depth: " << depth << "\n"; 
        // Search each position in this state
        for (auto const& move : board().free_tiles().binary_to_vector()) {
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

            // Decide if this is a better move
            if (search_evaluation < minEval) {
                minEval = search_evaluation;
                worstmove = move;
            }
            if (search_evaluation <= alpha) break;
            beta = std::min(beta, search_evaluation);

            search.unplay(move);
        }
        // std::cout << "Best (worst) move: " << worstmove << " with score " << minEval << '\n';
        return {minEval, worstmove};
    }


    return {-1, -1};
}