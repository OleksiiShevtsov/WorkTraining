#pragma once

#include <iostream>
#include <boost/asio.hpp>

void connectionCheck()
{
	boost::asio::io_context io_context;
	boost::asio::ip::tcp::resolver resolver{ io_context };
	boost::asio::ip::tcp::socket socket{ io_context };
	boost::asio::async_connect( socket, 
		resolver.resolve( "www.nostarch.com", "http" ),
		[]( boost::system::error_code ec, const auto& endpoint ){
			std::cout << endpoint;
		}
	);
	io_context.run();
	std::cout << std::endl;
}

/*void connectionCheck2()
{
	boost::asio::io_context io_context;
	boost::asio::ip::tcp::resolver resolver{ io_context };
	boost::asio::ip::tcp::socket socket{ io_context };
	try
	{
		auto endpoints = resolver.resolve( "www.nostarch.com", "http" );
		const auto connectedEndpoint = boost::asio::connect( socket, endpoints );
		std::cout << connectedEndpoint;
	}
	catch( boost::system::system_error& se )
	{
		std::cerr << "Error: " << se.what() << std::endl;
	}
}*/