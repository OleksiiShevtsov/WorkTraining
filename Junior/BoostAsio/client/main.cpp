#include <iostream>
#include "sync_client.h"

int main() {
    try {
        boost::asio::io_context io_context;
        SyncClient client(io_context, "127.0.0.1", 4000);
        client.SendAndReceive("Hello, Server!");
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}