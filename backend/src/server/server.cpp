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
#include "server_util.hpp"

#include "room.hpp"
#include "metadatagen.hpp"

using json = nlohmann::json;

void RelaySocket(){
	struct SocketData{
		//Empty because we don't need any currently.
	};
	
	uWS::App app = uWS::App();
	auto db = DatabaseManager();
	auto session_tokens = std::unordered_map<int, std::string>();
	
	app.listen(8080, [](auto *listen_socket){
		if(listen_socket){
			std::cout<< "Listening on port" << 8080 << std::endl;
		};
	});

	app.get("/api/david", [&app](auto *res, auto *req) {
		res->end("{\"name\": \"david\"}");
	});

	app.get("/api/snapshot", [&app](auto *res, auto *req) {
        for (auto header : *req){
            std::cout << header.first << ", " << header.second << "\n";
        };

		// auto query = req->getQuery();
		auto moves = std::string(req->getQuery("moves")); 
		auto metadata = MetaDataGenerator(moves, 2); // TODO: Hardcoded snapshot for 2 players
		auto const position = metadata.db_snapshot();
		std::cout << position.end()[-1] << '\n';
		std::cout << position.end()[-2] << '\n';

		


		res->end("{\"name\": \"test\"}");
	});

	app.post("/register", [&app, &db](auto *res, auto *req){
    
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
                         message =  "{\"event\": \"login\", \"action\": \"login\", \"payload\":" 
                            "{ \"outcome\" : \"failure\", \"message\": \"incorrect password\"}}";
                    // Login success
                    }else{
                    
                        if (session_tokens.find(user->id) == session_tokens.end()){
                            std::cout << "generating token\n";
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
                    message = "{\"event\": \"login\", \"action\": \"login\", \"payload\" :" 
                            "{ \"outcome\": \"failure\", \"message\": \"email not in database\"}}";
                }
                
                res->end(message);
			}
		});
		res->onAborted([]() -> void {});
	});

  app.get("/db", [&db](auto *res, auto *req) {
     // test inserting into db
		//  db.insert_user("username", "email", "password");
		//  auto user = db.get_user("email");
		//  res->end(user->password_hash);
		
				// test generate session token 
		res->end(generate_session_token(1));
		
		//auto matchID = db.save_match("CLASSIC", "12345");
		//res->end(std::to_string(matchID));
		
		
		auto matchID = db.save_match("CLASSIC", "12345");
		res->end(std::to_string(matchID));
		
		// testing get friends 
		auto friends = db.get_friends(1);
		auto friends_json = friends_to_json(1, friends);
		std::cout << " ***** freins to json\n";
		std::cout << friends_json;
		std::cout <<  "*****\n";
		
		// testing add friend
		auto add = db.add_friend(1,6);
		if (add){
			std::cout << "add 1 6 true\n";
		}else{
			std::cout << "add 1 6 false\n";

		}
		auto del = db.delete_friend(1,6);
		if (del){
			std::cout << "del 1 6 true\n";
		}else{
			std::cout << "del 1 6 false\n";
		}
		auto del2 = db.delete_friend(1,6);
		if (del2){
			std::cout << "del 1 6 true\n";
		}else{
			std::cout << "del 1 6 false\n";
		}
		
   });


	// Functions we have available to in socketing
	Room room = Room(app, &db, {1, 2});

	app.run();
}


int main()
{
	RelaySocket();
	return 0;
}
