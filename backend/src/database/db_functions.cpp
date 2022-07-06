#include <algorithm>
#include <string>
#include <unordered_set>
#include <iostream>
#include "db_classes.hpp"
#include "db_utils.hpp"

#include <pqxx/pqxx>
//#include <openssl/sha.h>

auto db_get_user(int id) -> User* {
	auto sql = std::string("SELECT * FROM users WHERE id = " + std::to_string(id) + ";");
	auto res = execute_query(sql);

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
	return user;
}

auto db_get_user(std::string email) -> User* {
	auto sql = std::string("SELECT * FROM users WHERE email = '" + email + "';");
	auto res = execute_query(sql);

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
	return user;
}

auto db_insert_user(std::string username, std::string email, std::string password) -> int {
	auto sql = std::string("INSERT INTO users VALUES (DEFAULT") + ", '" +
		username + "', '" + email + "', '" + password + "');";
	return commit_query(sql);
}

auto db_add_friends(int friend1, int friend2) -> int{
	if (friend1 == friend2){
		std::cerr << "Can't add self as friend\n";
		return -1;
	}
	auto sql = std::string("INSERT INTO friends VALUES (" + std::to_string(friend1) + ", " 
			+ std::to_string(friend2) + ");");
	return commit_query(sql);
}