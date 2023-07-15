#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>

using ResolvResult = boost::asio::ip::tcp::resolver::results_type;
using Endpoint = boost::asio::ip::tcp::endpoint;

struct Request 
{
	explicit Request(boost::asio::io_context& io_context, std::string host ) :
		m_resolver{ io_context },
		m_socket{ io_context },
		m_host{ std::move( host ) }
	{
		std::stringstream requestStream;
		requestStream << "GET / HTTP/1.1\r\n"
						 "Host: " << m_host << "\r\n"
						 "Accept: text/plain\r\n"
						 "Accept-Language: en-us\r\n"
						 "Accept-Encoding: identity\r\n"
						 "Connection: close\r\n";
						 "User-Agent: C++ Crash Course Client\r\n\r\n";
		m_request = requestStream.str();
		m_resolver.async_resolve(m_host, "http",
			[ this ]( boost::system::error_code ec, const ResolvResult& results ) 
			{
				resolutionHandler( ec, results );
			}
		);
	}

	void resolutionHandler( boost::system::error_code ec, const ResolvResult& results )
	{
		if( ec )
		{
			std::cerr << "Error resolving " << m_host << ": " << ec << std::endl;
			return;
		}
		boost::asio::async_connect( m_socket, results,
			[ this ]( boost::system::error_code ec, const Endpoint& endpoint )
			{
				connectionHandler( ec, endpoint );
			}
		);
	}

	void connectionHandler( boost::system::error_code ec, const Endpoint& endpoint )
	{
		if( ec )
		{
			std::cerr << "Error connecting to " << m_host << ": " << ec.message() << std::endl;
			return;
		}
		boost::asio::async_write( m_socket, boost::asio::buffer( m_request ),
			[ this ](boost::system::error_code ec, size_t transferred )
			{
				writeHandler( ec, transferred );
			}
		);
	}

	void writeHandler( boost::system::error_code ec, size_t transferred )
	{
		if( ec )
		{
			std::cerr << "Error writing to " << m_host << ": " << ec.message() << std::endl;
		}
		else if( m_request.size() != transferred )
		{
			m_request.erase( 0, transferred );
			boost::asio::async_write( m_socket, boost::asio::buffer( m_request ),
				[ this ]( boost::system::error_code ec, size_t transferred )
				{
					writeHandler( ec, transferred );
				}
			);
		}
		else
		{
			boost::asio::async_read( m_socket, boost::asio::dynamic_buffer( m_response ),
				[ this ]( boost::system::error_code ec, size_t transferred )
				{
					readHandler( ec, transferred );
				}
			);
		}
	}

	void readHandler( boost::system::error_code ec, size_t transferred)
	{
		if( ec && ec.value() != 2 )
		{
			std::cerr << "Error reading from " << m_host << ": " << ec.message() << std::endl;
			return;
		}
	}

	const std::string getResponse() const noexcept
	{
		return m_response;
	}

	private:

	boost::asio::ip::tcp::resolver m_resolver;
	boost::asio::ip::tcp::socket m_socket;
	std::string m_request, m_response;
	const std::string m_host;
};

void simplClientCheck()
{
	boost::asio::io_context io_context;
	Request request{ io_context, "www.arcyber.army.mil" };
	io_context.run();
	std::cout << request.getResponse();
}

/*std::string request(std::string host, boost::asio::io_context& io_context)
{
	std::stringstream requestStream;
	requestStream << "GET / HTTP/1.1\r\n"
					  "Host: " << host << "\r\n"
					  "Accept: text/html\r\n"
					  "Accept-Language: en-us\r\n"
					  "Accept-Encoding: identity\r\n"
					  "Connection: close\r\n\r\n";
	const auto request = requestStream.str();
	boost::asio::ip::tcp::resolver resolver{ io_context };
	const auto endpoints = resolver.resolve( host, "http" );
	boost::asio::ip::tcp::socket socket{ io_context };
	const auto connectedEndpoint = boost::asio::connect( socket, endpoints );
	boost::asio::write( socket, boost::asio::buffer( request ) );
	
	std::string response;
	boost::system::error_code ec;
	boost::asio::read( socket, boost::asio::dynamic_buffer( response ), ec );

	if (ec && ec.value() != 2) 
	{
		throw boost::system::system_error{ ec };
	}
	return response;
}

void requestCheck()
{
	boost::asio::io_context io_context;
	try
	{
		const auto response = request( "www.arcyber.army.mil", io_context );
		std::cout << response << "\n";
	}
	catch ( boost::system::system_error& se )
	{
		std::cerr << "Error: " << se.what() << std::endl;
	}
}*/