#ifndef SERVER_API_HPP
#define SERVER_API_HPP

#include "App.h"
#include "db_manager.hpp"

// POST REQUESTS
auto registerPage(uWS::App &app, DatabaseManager &db) -> void;
auto login(uWS::App &app, DatabaseManager &db, std::unordered_map<int, std::string> &session_tokens) -> void;
auto friendaction(uWS::App &app, DatabaseManager &db, std::unordered_map<int, std::string> &session_tokens) -> void;

// GET REQUESTS
auto api_profile(uWS::App &app, DatabaseManager &db, std::unordered_map<int, std::string> &session_tokens) -> void;
auto api_replay(uWS::App &app, DatabaseManager &db) -> void;
auto api_search_all(uWS::App &app, DatabaseManager &db) -> void;
auto api_search_snapshot(uWS::App &app, DatabaseManager &db) -> void;
auto api_friends(uWS::App &app, DatabaseManager &db, std::unordered_map<int, std::string> &session_tokens) -> void;
auto api_leaderboards(uWS::App &app, DatabaseManager &db) -> void;

// TESTING ENDPOINTS
auto api_david(uWS::App &app) -> void;
auto api_db(uWS::App &app, DatabaseManager &db) -> void;

#endif