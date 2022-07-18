#ifndef SERVER_ROOM_HPP
#define SERVER_ROOM_HPP

#include <string>
#include "App.h"

#include "game.hpp"
#include "aigame.hpp"
#include "db_manager.hpp"

class Room {
    private:
        std::string room_id_;
        std::vector<int> uids_; 
        std::unique_ptr<Game> game_;
        std::unique_ptr<AIGame> aigame_;
        DatabaseManager *db_;
        bool ranked_;
        bool computer_;

    public:
        Room(uWS::App &app, DatabaseManager *db, bool ranked, bool computer, std::string room_id, std::vector<int> uids);

        auto room_id() const -> std::string;
        auto room_code() const -> std::string;

        auto publish_move(std::string const& move) {
            if (game_ == nullptr) std::cout << "Lobby: WHAT IS HAPPENING\n";
            auto player = game_->whose_turn();
            if (player == 0) return json_confirm_move(move);
            if (player == 1) return json_board_move(move);
            if (player == 2) return json_confirm_move(move); // TODO: CHANGE
            return json_confirm_move(move); // TODO: UNREACHABLE IDEALLY
        }

        auto game_result() -> std::string;
		auto game_result(int const uid) -> std::string;

    private:
        auto generate_game() -> void;       // Init game
        auto create_socket_ai(uWS::App &app) -> void;    // Create room to verse ai
        auto create_socket_player_verse_player(uWS::App &app) -> void;

        auto play_move(std::string const& move) -> bool;
        auto ai_response(std::string const& move) -> std::string;

        auto json_confirm_move(std::string const& move) -> std::string;
        auto json_board_move(std::string const& move) -> std::string;
        auto json_game_winner(std::string const& player) -> std::string;

        auto calc_elos(int winning_player) -> std::map<int, int>;
        auto save_match(int winning_player) -> void;
};

#endif