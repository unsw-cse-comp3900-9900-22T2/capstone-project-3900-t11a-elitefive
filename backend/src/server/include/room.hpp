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

    public:
        Room(uWS::App &app, DatabaseManager *db, std::vector<int> uids);

        auto room_id() const -> std::string;

    private:
        auto generate_game() -> void;       // Init game
        auto create_socket_ai(uWS::App &app) -> void;    // Create room to verse ai

        auto play_move(std::string const& move) -> bool;
        auto ai_response(std::string const& move) -> std::string;
};

#endif