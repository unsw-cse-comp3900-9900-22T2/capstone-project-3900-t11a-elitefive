#include <algorithm>
#include <string>
#include <unordered_set>
#include <iostream>
#include "db_classes.hpp"

#include <pqxx/pqxx>
//#include <openssl/sha.h>

// todo: config with docker & not sage's laptop
auto const DB_STRING = std::string("user=yavalath dbname=yavalath password=yavalath host=postgres port=5432");

auto db_get_user(int id) -> User* {

	try{
		auto sql = std::string("SELECT * FROM users WHERE id = " + std::to_string(id) + ";");
	
		pqxx::connection conn(DB_STRING);
		pqxx::work w(conn);
		auto res = w.exec(sql);
		
		auto num_tups = res.size();
		
		// If no user found (or more than 1 which is impossible)
		if (num_tups != 1){
			return NULL;
		}
		
		// User class from tuple
		auto user = new User;
		user->id = atoi(res[0][0].c_str());
		user->username = std::string(res[0][1].c_str());
		user->email = std::string(res[0][2].c_str());
		user->password_hash = std::string(res[0][3].c_str());
		
		conn.disconnect();
		
		return user;
	}
	catch(const pqxx::pqxx_exception &e){
		std::cerr  << e.base().what();
		return NULL;
	}
}

auto db_get_user(std::string email) -> User* {

	try{
		auto sql = std::string("SELECT * FROM users WHERE email = '" + email + "';");
	
		pqxx::connection conn(DB_STRING);
		pqxx::work w(conn);
		auto res = w.exec(sql);
		
		auto num_tups = res.size();
		
		// If no user found (or more than 1 which is impossible)
		if (num_tups != 1){
			return NULL;
		}
		
		// User class from tuple
		auto user = new User;
		user->id = atoi(res[0][0].c_str());
		user->username = std::string(res[0][1].c_str());
		user->email = std::string(res[0][2].c_str());
		user->password_hash = std::string(res[0][3].c_str());
		
		conn.disconnect();
		
		return user;
	}
	catch(const pqxx::pqxx_exception &e){
		std::cerr  << e.base().what();
		return NULL;
	}
}

auto db_add_friends(int friend1, int friend2) -> int{
	
	if (friend1 == friend2){
		std::cerr << "Can't add self as friend\n";
		return -1;
	}
	
	try {
		auto sql = std::string("INSERT INTO friends VALUES (" + std::to_string(friend1) + ", " 
			+ std::to_string(friend2) + ");");
			
		pqxx::connection conn(DB_STRING);
		pqxx::work w(conn);
		w.exec(sql);
		w.commit();
		
		conn.disconnect();
		
		return 0;
	}
	catch(const pqxx::pqxx_exception &e){
		std::cerr  << e.base().what();
		return -1;
	}
}


auto hash_password(std::string password) -> std::string {

	// TODO: figure out how to add this lib
	/*
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
    */
    return password;
}


auto db_insert_user(std::string username, std::string email, std::string password) -> int {

	try{
		auto sql = std::string("INSERT INTO users VALUES (DEFAULT") + ", '" +
		username + "', '" + email + "', '" + hash_password(password) + "');";
	
		pqxx::connection conn(DB_STRING);
		pqxx::work w(conn);
		w.exec(sql);
		w.commit();
		
		conn.disconnect();
		return 0;
	}
	catch(const pqxx::pqxx_exception &e){
		std::cerr  << e.base().what();
		return -1;
	}
}

