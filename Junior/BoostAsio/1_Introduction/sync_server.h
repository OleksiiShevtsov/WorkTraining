#pragma once

#include <boost/asio.hpp>
#include <boost/memory_order.hpp>

//#include <boost/thread.hpp>
#include <thread>

using SocketPtr = boost::shared_ptr<boost::asio::ip::tcp::socket>;

void clientSession(SocketPtr sock){
	while(true){
		char data[512];
		size_t len = sock->read_some(boost::asio::buffer(data));
		if(len > 0){
			write(*sock, boost::asio::buffer("ok", 2));
		}
	}
}

void asyncServerStart()
{
	boost::asio::io_service service;
	boost::asio::ip::tcp::endpoint ep(
		boost::asio::ip::address::from_string("127.0.0.1"), 2001);
	boost::asio::ip::tcp::acceptor acc(service, ep);

	
	while(true){
		SocketPtr sock(new boost::asio::ip::tcp::socket(service));
		acc.accept(*sock);

		std::thread(clientSession, sock);
		//boost::thread(boost::bind(clientSession, sock));
	}
}