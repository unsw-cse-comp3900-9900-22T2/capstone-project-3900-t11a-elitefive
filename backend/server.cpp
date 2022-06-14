#include <iostream>
#include <uwebsockets/App.h>
#include <libusockets.h>
// #include <uwebsockets/HttpContext.h>
// #include <uwebsockets/AsyncSocket.h>

int main(int argc, char const *argv[])
{
  uWS::App().get("/api/david", [](auto *res, auto *req) {
    res->end("{\"name\": \"david\"}");
  }).listen(8080, [](auto *listenSocket) {
    if(listenSocket) {
        std::cout << "Listening for connections..." << std::endl;
    }
  }).run();
}
