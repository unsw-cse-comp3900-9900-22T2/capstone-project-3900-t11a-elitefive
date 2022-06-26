#include <iostream>
#include <uwebsockets/App.h>
#include <uwebsockets/WebSocket.h>
#include <libusockets.h>
// #include <uwebsockets/HttpContext.h>
// #include <uwebsockets/AsyncSocket.h>

void RelaySocket(){
    struct SocketData{
        //Empty because we don't need any currently.
    };
    
    auto app = uWS::App();
    
    app.listen(8080, [](auto *listen_socket){
        if(listen_socket){
            std::cout<< "Listening on port" << 8080<< std::endl;
        };
    });

    app.get("/api/david", [&app](auto *res, auto *req) {
      res->end("{\"name\": \"david\"}");
    });


    auto websocket = app.ws<SocketData>("/ws/david",uWS::TemplatedApp<false>::WebSocketBehavior<SocketData> {//I have to explicitly declare the type of this struct.
      .open = [](auto *ws) {
        // std::cout<< "test"<< std::endl;
        // std::cout << ws->getRemoteAddressAsText() << '\n';
        ws->subscribe("moves");
        // std::cout << "subbed: " << ws->isSubscribed("moves") << '\n';
      },
      
      .message = [](auto *ws, std::string_view message, uWS::OpCode opCode){
        std::cout << message << '\n';
        //The docs show how to send messages from this context, but no other method is demonstrated.
        // ws->send("Hello there boi");// This works fine enough.
        ws->publish("moves", 
          "{\"turn\": \"whose turn\", \"last move\": \"here\"}", 
          opCode);
      },
      .close = [](auto *ws, int x , std::string_view str) {
        std::cout << "close occured" << '\n';
        std::cout << x << ' ' << str << '\n';
        ws->close();
      }


    });

    app.run();
}

int main()
{
    RelaySocket();
    return 0;
}
