#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class SyncServer {
public:
    SyncServer(boost::asio::io_context& io_context, unsigned short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
        , socket_(io_context) {}

    void Start() {
        std::cout << "server start..." << std::endl;

        acceptor_.accept(socket_);

        std::cout << "client acceptor:" << std::endl;

        HandleClient();
    }

private:
    void HandleClient() {
        try {
            char data[1024];
            boost::system::error_code error;

            size_t length = socket_.read_some(boost::asio::buffer(data), error);

            if (error == boost::asio::error::eof) {
                std::cout << "client shutdown:" << std::endl;
                return;
            }
            else if (error) {
                throw boost::system::system_error(error);
            }

            std::cout << "message received from client: " << std::string(data, length) << std::endl;

            boost::asio::write(socket_, boost::asio::buffer(data, length));
        }
        catch (std::exception& e) {
            std::cerr << "client processing error: " << e.what() << std::endl;
        }
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
};