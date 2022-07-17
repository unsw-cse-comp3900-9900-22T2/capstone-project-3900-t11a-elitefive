#include "api.hpp"
#include "App.h"
#include "db_manager.hpp"
#include "db_utils.hpp"
#include "server_util.hpp"

using json = nlohmann::json;

// POST REQUESTS
auto registerPage(uWS::App &app, DatabaseManager &db) -> void {
	app.post("/register", [&app, &db](auto *res, auto *req) {
		// Loop through header 
		for (auto header : *req){
			std::cout << header.first << ", " << header.second << "\n";
		};
		
		// Get data from request
		res->onData([&db, res](std::string_view data, bool last) {
			auto buffer = std::string("");
			auto message = std::string("");
			
			buffer.append(data.data(), data.length());
			if (last) {
				auto user_json = json::parse(data);
				auto username = std::string(user_json["username"]);
				auto email = std::string(user_json["email"]);
				auto password = std::string(user_json["password"]);


				// Register Success
				if (db.insert_user(username, email, password)){
					message = "{\"event'\": \"register\", \"action\": \"register\","
					"\"payload\": { \"outcome\" : \"success\"}}";
				}
				// Register Failure
				else{
					message = "{\"event'\": \"register\", \"action\": \"register\", "
						"\"payload\": { \"outcome\" : \"failure\"}}";
				}
				// Respond
				res->end(message);
			}
		});
		res->onAborted([]() -> void {});
	});
}

auto login(uWS::App &app, DatabaseManager &db, std::unordered_map<int, std::string> &session_tokens) -> void {	
	app.post("/login", [&app, &db, &session_tokens](auto *res, auto *req){
		// Get data from request
		res->onData([&db, res, &session_tokens](std::string_view data, bool last) {
			auto buffer = std::string("");
			auto message = std::string("");
			
			buffer.append(data.data(), data.length());
			if (last) {
				auto user_json = json::parse(data);
				auto email = std::string(user_json["email"]);
				auto password = std::string(user_json["password"]); 
		
				auto user = db.get_user(email);

				if (user != NULL){
					// Incorrect password
					if (hash_password(password) != user->password_hash){
						std::cout << "LOGIN: incorrect login\n";
						 message =  "{\"event\": \"login\", \"action\": \"login\", \"payload\":" 
							"{ \"outcome\" : \"failure\", \"message\": \"incorrect password\"}}";
					// Login success
					}else{
						std::cout << "LOGIN: successful login\n";
						if (session_tokens.find(user->id) == session_tokens.end()){
							auto session_token = generate_session_token(user->id);
							session_tokens[user->id] = session_token;
						}
					
						message =  std::string("{\"event\": \"login\", \"action\": \"login\", \"payload\":") +  
							std::string("{ \"outcome\" : \"success\", \"uid\": \"") +  std::to_string(user->id) +
							std::string("\", \"email\": \"") + user->email + std::string("\" ") + ","+
							std::string ("\"session\": \"") + session_tokens[user->id] + std::string("\"}}");
					}
				// Email doesn'e exist
				}else{
					std::cout << "LOGIN: Doesn't exist\n";
					message = "{\"event\": \"login\", \"action\": \"login\", \"payload\" :" 
							"{ \"outcome\": \"failure\", \"message\": \"email not in database\"}}";
				}
				
				res->end(message);
			}
		});
		res->onAborted([]() -> void {});
	});
}

// GET REQUESTS
auto api_profile(uWS::App &app, DatabaseManager &db) -> void {

}
auto api_replay(uWS::App &app, DatabaseManager &db) -> void {

}
auto api_search_all(uWS::App &app, DatabaseManager &db) -> void {

}
auto api_search_snapshot(uWS::App &app, DatabaseManager &db) -> void {

}

// TESTING ENDPOINTS
auto api_david(uWS::App &app) -> void {

}
auto api_db(uWS::App &app, DatabaseManager &db) -> void {

}