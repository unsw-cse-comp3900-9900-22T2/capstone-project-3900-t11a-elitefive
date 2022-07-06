#ifndef DATABASE_UTILS_HPP
#define DATABASE_UTILS_HPP

#include "db_utils.hpp"

#include <pqxx/pqxx>

// todo: config with docker & not sage's laptop
auto const DB_STRING = std::string("user=yavalath dbname=yavalath password=yavalath host=postgres port=5432");

auto execute_query(std::string sql) -> pqxx::result {
	try {
		pqxx::connection conn(DB_STRING);
		pqxx::work w(conn);
		auto res = w.exec(sql);
		conn.disconnect();
		return res;
	} catch (const pqxx::pqxx_exception &e) {
		std::cerr << e.base().what();
		return pqxx::result{};
	}
}

auto commit_query(std::string sql) -> int {
	try {
		pqxx::connection conn(DB_STRING);
		pqxx::work w(conn);
		w.exec(sql);
		w.commit();
		conn.disconnect();
		return 0;
	} catch (const pqxx::pqxx_exception &e) {
		std::cerr << e.base().what();
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

#endif