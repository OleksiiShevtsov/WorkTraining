#pragma once

#include <boost/asio.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <future>

struct Session : std::enable_shared_from_this< Session >
{
	explicit Session( boost::asio::ip::tcp::socket socket )
		: m_socket{ std::move( socket ) }{}

	void read() 
	{
		boost::asio::async_read_until( m_socket,
			boost::asio::dynamic_buffer( m_message ), '\n',
			[ self = shared_from_this() ](
				boost::system::error_code ec,
				std::size_t length)
			{
				if( ec || self->m_message == "\n" ) { return; }
				boost::algorithm::to_upper( self->m_message );
				self->write();
			}
		);
	}

	void write()
	{
		boost::asio::async_write( m_socket,
			boost::asio::buffer( m_message ),
			[ self = shared_from_this() ](
				boost::system::error_code ec,
				std::size_t length )
			{
				if( ec ){ return; }
				self->m_message.clear();
				self->read();
			}
		);
	}

	private:
	
	boost::asio::ip::tcp::socket m_socket;
	std::string m_message;
};

void server( boost::asio::ip::tcp::acceptor& acceptor )
{
	acceptor.async_accept(
		[ &acceptor ]( boost::system::error_code ec,
					   boost::asio::ip::tcp::socket socket )
		{
			server( acceptor );
			if( ec ){ return; }
			auto session = std::make_shared< Session >( std::move( socket ) );
			session->read();
		}
	);
}

void simplServerCheck()
{
	std::cout << "start server..." << std::endl;

	const int nThreads{ 4 };
	boost::asio::io_context io_context{ nThreads };
	boost::asio::ip::tcp::acceptor acceptor
	{
		io_context,
		boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), 1895 )
	};

	server( acceptor );

	std::vector< std::future< void > > futures;
	std::generate_n( std::back_inserter( futures ), nThreads,
		[ &io_context ]
		{
			return std::async( std::launch::async,
				[ &io_context ]
				{
					io_context.run();
				}
			);
		}
	);

	for ( auto& future : futures ) {
		try
		{
			future.get();
		}
		catch ( std::exception& e )
		{
			std::cerr << e.what() << std::endl;
		}
	}
}

/*void handle(boost::asio::ip::tcp::socket& socket)
{
	boost::system::error_code ec;
	std::string message;
	do
	{
		boost::asio::read_until( socket, boost::asio::dynamic_buffer( message ), "\n" );
		boost::algorithm::to_upper( message );
		boost::asio::write( socket, boost::asio::buffer( message ), ec );
		if( message == "\n" )
		{
			return;
		}
		message.clear();
	} 
	while( !ec );
}

void simplServerCheck()
{
	std::cout << "start server..." << std::endl;
	try
	{
		boost::asio::io_context io_context;
		boost::asio::ip::tcp::acceptor acceptor
		{
			io_context,
			boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), 1895 )
		};

		while( true )
		{
			boost::asio::ip::tcp::socket socket{ io_context };
			acceptor.accept( socket );
			handle( socket );
		}
	}
	catch( std::exception& e )
	{
		std::cerr << e.what() << std::endl;
	}
}*/
