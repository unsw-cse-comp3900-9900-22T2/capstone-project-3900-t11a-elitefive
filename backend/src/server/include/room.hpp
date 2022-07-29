#ifndef SERVER_ROOM_HPP
#define SERVER_ROOM_HPP

#include <string>
#include "App.h"

#include "game.hpp"
#include "aigame.hpp"
#include "db_manager.hpp"


class Room {
    struct SocketData{
        //Empty because we don't need any currently.
    };
    typedef uWS::WebSocket<false, true, SocketData> *WebSocket;
    private:
        std::string room_id_;
        int gamemode_;
        std::vector<int> uids_; 
        std::unique_ptr<Game> game_;
        std::unique_ptr<AIGame> aigame_;
        DatabaseManager *db_;
        bool ranked_;
        bool computer_;

    public:
        Room(uWS::App &app, DatabaseManager *db, bool ranked, bool computer, bool potholes, std::string room_id, std::vector<int> uids);
        auto InitRoom(bool ranked, bool computer, bool potholes) -> void;
        
        auto room_id() const -> std::string { return room_id_; }
        auto room_code() const -> std::string { return std::string{"/ws/game/" + this->room_id()}; }
        auto play_move(std::string const& move) -> bool { return this->game_->play(move); }
        auto gamemode() const -> std::string { 
            if (gamemode_ == 0) return "CLASSIC"; 
            if (gamemode_ == 1) return "TRIPLES"; 
            if (gamemode_ == 2) return "POTHOLES";
        }
        
        auto publish_move(std::string const& move) {
            if (game_ == nullptr) std::cout << "Lobby: WHAT IS HAPPENING\n";
            auto player = game_->whose_turn();
            if (player == 0) return json_confirm_move(move);
            if (player == 1) return json_board_move(move);
            if (player == 2) return json_player3(move); // TODO: CHANGE
            return json_confirm_move(move); // TODO: UNREACHABLE IDEALLY
        }

        auto game_result() -> std::string;
		auto game_result(int const uid) -> std::string;
        
        auto on_connect_match_info(uWS::WebSocket<false, true, SocketData> *ws) -> void;

    private:
        auto generate_game(bool potholes) -> void;       // Init game
        auto create_socket_ai(uWS::App &app) -> void;    // Create room to verse ai
        auto create_socket_player_verse_player(uWS::App &app) -> void;

        auto static ai_response(std::string move, AIGame *aigame, Game *game, void *ws) -> std::string;
        auto json_confirm_move(std::string const& move) -> std::string;
        auto json_player3(std::string const& move) -> std::string;
        auto json_pothole(std::string const& move) -> std::string;
        auto json_board_move(std::string const& move) -> std::string;
        auto json_game_winner(std::string const& player) -> std::string;

        auto calc_elos(int winning_player) -> std::map<int, int>;
        auto save_match(int winning_player) -> void;

    private:
        auto publish(uWS::WebSocket<false, true, SocketData> *ws, std::string const& message, uWS::OpCode opCode) -> void;

        auto click_register_move(std::string const& move, uWS::WebSocket<false, true, SocketData> *ws, uWS::OpCode opCode) -> void;


        auto click_from_players_turn(int uid) -> bool {
			int players_turn = this->game_->whose_turn();
			int uid_turn = this->game_->give_uid(players_turn);
			return (uid_turn == uid);
        }
};

#endif