#pragma once

#include <boost/asio.hpp>
#include <thread>
#include <string>
#include <iostream>

const int MaxBufferLength = 1024;

class NotificationClient
{
public:
	NotificationClient(
		boost::asio::io_context& io_context, 
		std::string host,
		std::string port ) : 
		m_resolver{ io_context },
		m_socket{ io_context },
		m_host{ std::move( host ) },
		m_port{ std::move( port ) }
	{
		boost::system::error_code ec;
		const auto endpoints = m_resolver.resolve( m_host, m_port, ec );
		if( ec ){ throw  "Error resolving " + m_host; }
		const auto connectedServerEndpoints = boost::asio::connect( m_socket, endpoints, ec );
		if( ec ){ throw  "Error connecting to " + m_host + ": " + ec.message(); }
		m_serverMessageHandler = std::make_unique< std::thread >( &NotificationClient::serverMessageHandler, this );
	}

	~NotificationClient()
	{
		m_serverMessageHandler->join();
	}

private:

	void serverMessageHandler()
	{
		try
		{
			char buffer[MaxBufferLength];

			while (true) 
			{
				/* boost::asio::async_read(m_socket, boost::asio::dynamic_buffer(m_response),
					[ this ]( const boost::system::error_code error, size_t bytes_received ) {
						if ( !error ) 
						{
							std::cout << "Сообщение от сервера: " << std::string( m_response, bytes_received ) << std::endl;
						}
						else 
						{
							std::cerr << "Error: " << error.message() << std::endl;
						}
					}); */

				// static_cast< boost::asio::io_context& >( m_socket.get_executor().context() ).run();
				std::size_t bytestransferred = 
				m_socket.read_some( boost::asio::buffer( buffer ) );

				std::string message( buffer, bytestransferred );
				std::cout << "Received message: " << message << std::endl;
			}
		}
		catch ( const std::exception& ex ) 
		{
			std::cerr << "Error: " << ex.what() << std::endl;
		}
	}

	std::unique_ptr< std::thread > m_serverMessageHandler;
	boost::asio::ip::tcp::resolver m_resolver;
	boost::asio::ip::tcp::socket m_socket;
	std::string m_response;
	const std::string m_host;
	const std::string m_port;
};