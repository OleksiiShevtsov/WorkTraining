#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>

//#include <boost/thread.hpp>
#include <thread>

using SocketPtr = boost::shared_ptr<boost::asio::ip::tcp::socket>;

void syncClientStart()
{
	boost::asio::io_service service;
	boost::asio::ip::tcp::endpoint ep(
		boost::asio::ip::address::from_string("127.0.0.1"), 2001);
	boost::asio::ip::tcp::acceptor acc(service, ep);

	SocketPtr sock(new boost::asio::ip::tcp::socket(service));

	acc.async_accept(sock);



	service.run();
}