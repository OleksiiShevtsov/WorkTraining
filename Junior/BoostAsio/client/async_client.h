#pragma once

#include <boost/asio.hpp>

void connectHandler(const boost::system::error_code& ec)
{
	// here we know we connected successfully
	// if ec indicates success 
}

void syncClientStart()
{
	boost::asio::io_service service;
	boost::asio::ip::tcp::endpoint ep(
		boost::asio::ip::address::from_string("127.0.0.1"), 2001);
	boost::asio::ip::tcp::socket sock(ep, connectHandler);
}