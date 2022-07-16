#ifndef DATABASE_UTILS_HPP
#define DATABASE_UTILS_HPP

#include "db_utils.hpp"
#include "db_classes.hpp"

#include <pqxx/pqxx>
#include <openssl/sha.h>

auto hash_password(std::string password) -> std::string {

	// TODO: figure out how to add this lib
	auto c_password = password.c_str();
	char buffer[64];
	
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, c_password, strlen(c_password));
    SHA256_Final(hash, &sha256);
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
        sprintf(buffer + (i * 2), "%02x", hash[i]);
    }
    buffer[64] = 0;
    
    return std::string(buffer);
    return password;
}



auto check_password(std::string email, std::string password) -> bool {

    return false;
}

auto friends_to_json(int id, std::vector<User*> friends) -> std::string {
    
    auto friends_json = std::string(
    "{"
    	"\"event\": \"friends\","
    	"\"action\": \"get\","
    	"\"payload\": {"
    		"\"user\": \"" + std::to_string(id) + "\", "
    		"\"friends\": ["
    );
    
    for (auto fri : friends){
        friends_json = friends_json + 
        "{"
			"\"uid\":" +  std::to_string(fri->id)  + ","
			"\"username\": \"" + fri->username + "\"" + 
		"},";
    }
    
    friends_json.pop_back();
    friends_json += "]}}";
            
    return friends_json;
}

#endif