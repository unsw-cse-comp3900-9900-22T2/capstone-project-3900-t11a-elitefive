#include "basegame.hpp"

BaseGame::BaseGame(int players)
: board_{Board(players)}
, player_turn_{0}
, nmoves_{0}
{}

auto BaseGame::whose_turn() -> int {
    return player_turn_;
}
auto BaseGame::num_moves() -> int {
    return nmoves_;
}

auto BaseGame::board() -> Board & {
    return board_;
}

auto BaseGame::board() const -> Board const& {
    return board_;
}

auto BaseGame::increase_move() -> void {
    ++nmoves_;
}

auto BaseGame::pass_turn() -> void {
    player_turn_ = (player_turn_ + 1) % board().num_players();
}