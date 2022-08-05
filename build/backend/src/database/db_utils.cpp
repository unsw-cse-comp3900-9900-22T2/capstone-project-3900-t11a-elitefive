#ifndef DATABASE_UTILS_HPP
#define DATABASE_UTILS_HPP

#include "db_utils.hpp"
#include "db_classes.hpp"

#include <pqxx/pqxx>
#include <openssl/sha.h>

using json = nlohmann::ordered_json;

auto hash_password(std::string password) -> std::string {
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

#endif