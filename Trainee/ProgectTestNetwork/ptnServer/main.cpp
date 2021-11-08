#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

int main(){

    try {
        boost::asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 1337));

        std::cout << 
        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::string msg = "server msg";

            boost::system::error_code error;

            boost::asio::write(socket, boost::asio::buffer(msg), error);
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl; 
    }

    return 0;
}
