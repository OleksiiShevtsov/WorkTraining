#pragma once

#include <boost/algorithm/string/case_conv.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <deque>
#include <future>

namespace alexgram
{
	struct OutputNotificationQueue
	{
		OutputNotificationQueue(
			boost::asio::io_context& io_context,
			int port) :
			m_io_context{ io_context },
			m_port{ std::move(port) }
		{
			m_acceptor = std::make_unique< boost::asio::ip::tcp::acceptor >(
				m_io_context,
				boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), m_port)
			);
		}

		void handleClient()
		{
			while (true)
			{
				boost::asio::ip::tcp::socket socket(m_io_context);
				m_acceptor->accept(socket);

				std::cout << "Client connected." << std::endl;

				m_clientThreads.emplace_back(&OutputNotificationQueue::listenToMessageFromClient, this, std::move(socket));
			}
		}

		~OutputNotificationQueue()
		{
			for (auto& thread : m_clientThreads) {
				thread.join();
			}
		}

	private:

		void listenToMessageFromClient(boost::asio::ip::tcp::socket& socket)
		{
			while (socket.is_open())
			{
				try
				{
					std::this_thread::sleep_for(std::chrono::seconds(2));
					std::string message = "Hello client";
					boost::asio::write(socket, boost::asio::buffer(message));
				}
				catch (const std::exception& ex)
				{
					std::cerr << "Error: " << ex.what() << std::endl;
					socket.close();
				}
			}
		}

		const int m_port;
		boost::asio::io_context& m_io_context;
		std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;
		std::vector<std::thread> m_clientThreads;
	};
}