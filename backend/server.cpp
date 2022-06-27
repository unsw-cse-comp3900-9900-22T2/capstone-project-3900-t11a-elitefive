#include <iostream>
#include <uwebsockets/App.h>
#include <libusockets.h>
// #include <uwebsockets/HttpContext.h>
// #include <uwebsockets/AsyncSocket.h>
#include "db_functions.hpp"

#include <pqxx/pqxx>

int main(int argc, char const *argv[])
{
  uWS::App()
  .get("/api/david", [](auto *res, auto *req) {
    res->end("{\"name\": \"david\"}");
  })
  .get("/db", [](auto *res, auto *req) {
  
    // test inserting into db
    db_insert_user("jessie" ,"jessie@jessie.com", "meowth");
    
    res->end("pgg");
  })
  .listen(8080, [](auto *listenSocket) {
    if(listenSocket) {
        std::cout << "Listening for connections..." << std::endl;
    }
  }).run();
}