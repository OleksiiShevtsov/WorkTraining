#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

class SyncClient {
public:
    SyncClient(boost::asio::io_context& io_context, const std::string& host, unsigned short port)
        : socket_(io_context) {
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
        boost::asio::connect(socket_, endpoints);
    }

    void SendAndReceive(const std::string& message) {
        try {
            boost::asio::write(socket_, boost::asio::buffer(message));

            char reply[1024];
            boost::system::error_code error;
            size_t reply_length = boost::asio::read(socket_, boost::asio::buffer(reply, message.size()), error);

            if (error == boost::asio::error::eof) {
                std::cout << "Server disconnected." << std::endl;
            }
            else if (error) {
                throw boost::system::system_error(error);
            }

            std::cout << "Server response: " << std::string(reply, reply_length) << std::endl;
        }
        catch (std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

private:
    tcp::socket socket_;
};