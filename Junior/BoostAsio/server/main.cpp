#include <iostream>
//#include "sync_server.h"
#include "async_server.h"

int main() {
    try {
        boost::asio::io_context io_context;
        //SyncServer server(io_context, 4000);
        //server.Start();
        AsyncServer server(io_context, 4000);
        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "������: " << e.what() << std::endl;
    }

    return 0;
}