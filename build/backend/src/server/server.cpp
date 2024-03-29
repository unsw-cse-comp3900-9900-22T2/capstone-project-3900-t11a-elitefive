// Normal includes
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

// External libs
#include <nlohmann/json.hpp>
#include <pqxx/pqxx>
#include "App.h"

// Our libs
#include "board.hpp"
#include "computer.hpp"
#include "db_utils.hpp"
#include "db_manager.hpp"
#include "aigame.hpp"

#include "room.hpp"
#include "pool.hpp"
#include "api.hpp"
#include "server_util.hpp"
#include "metadatagen.hpp"
#include "invite.hpp"

#include <stdio.h>

using json = nlohmann::ordered_json;

void BackendServer() {
	uWS::App app = uWS::App();
	auto database_connection = DatabaseManager();
	auto session_tokens = std::unordered_map<int, std::string>();
	

	auto &db = database_connection;
	auto &tokens = session_tokens;

	app.listen(8080, [](auto *listen_socket){
		if(listen_socket){
			std::cout<< "Listening on port" << 8080 << std::endl;
		};
	});

	// Post Requests
	registerPage(app, db);
	login(app, db, tokens);
	friendaction(app, db, tokens);
	changePW(app, db);
	tempPass(app, db);
	resetPass(app,db);

	// Get Requests
	api_search_snapshot(app, db);
	api_search_all(app, db);
	api_profile(app, db, tokens);
	api_replay(app, db);
	api_friends(app, db, tokens);
	api_leaderboards(app, db);
	api_social_feed(app, db);

	// Test functionality
	api_david(app);
	api_db(app, db);

	printf("DB Pointer: %p\n", &database_connection);

	Pool pool = Pool(&app, &database_connection);
	Invite invite = Invite(&app, &database_connection);
	
	app.run();
}


int main()
{
	BackendServer();
	return 0;
}
