#include <boost/asio.hpp>
#include <iostream>
#include <memory>

using boost::asio::ip::tcp;

class AsyncServer {
public:
    AsyncServer(boost::asio::io_context& io_context, unsigned short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
        , io_context_(io_context) {
        StartAccept();
    }

private:
    void StartAccept() {
        auto new_socket = std::make_shared<tcp::socket>(io_context_);
        acceptor_.async_accept(*new_socket, [this, new_socket](const boost::system::error_code& error) {
            if (!error) {
                std::cout << "Клиент подключен." << std::endl;
                HandleClient(new_socket);
            }
            StartAccept(); // Принимаем следующее подключение
            });
    }

    void HandleClient(std::shared_ptr<tcp::socket> socket) {
        auto data = std::make_shared<std::vector<char>>(1024);
        socket->async_read_some(boost::asio::buffer(*data),
            [this, socket, data](const boost::system::error_code& error, std::size_t length) {
                if (!error) {
                    std::cout << "Получено сообщение от клиента: " << std::string(data->data(), length) << std::endl;
                    SendResponse(socket, data, length);
                }
                else {
                    std::cerr << "Ошибка чтения: " << error.message() << std::endl;
                }
            });
    }

    void SendResponse(std::shared_ptr<tcp::socket> socket, std::shared_ptr<std::vector<char>> data, std::size_t length) {
        boost::asio::async_write(*socket, boost::asio::buffer(*data, length),
            [this, socket, data](const boost::system::error_code& error, std::size_t /*length*/) {
                if (!error) {
                    std::cout << "Ответ отправлен клиенту." << std::endl;
                }
                else {
                    std::cerr << "Ошибка отправки: " << error.message() << std::endl;
                }
            });
    }

    tcp::acceptor acceptor_;
    boost::asio::io_context& io_context_;
};