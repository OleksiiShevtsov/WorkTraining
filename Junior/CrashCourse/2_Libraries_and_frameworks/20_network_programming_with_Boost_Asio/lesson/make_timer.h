#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <chrono>

boost::asio::steady_timer makeTimer( boost::asio::io_context& io_context )
{
	return boost::asio::steady_timer{
		io_context,
		std::chrono::steady_clock::now() + std::chrono::seconds{ 3 }
	};
}

void makeTimerCheck()
{
	boost::asio::io_context io_context;
	
	auto timer1 = makeTimer(io_context );
	std::cout << "entering stedy_timer::wait\n";
	timer1.wait();
	std::cout << "exited stedy_timer::wait\n";

	auto timer2 = makeTimer( io_context );
	std::cout << "entering stedy_timer::async_wait\n";
	timer2.async_wait( [] ( const boost::system::error_code& error )
		{
			if ( !error ) std::cout << "<< callback function >>\n";
		}
	);
	std::cout << "exited stedy_timer::async_wait\n";

	std::cout << "entering io_context::run\n";
	io_context.run();
	std::cout << "exited io_context::run\n";
}