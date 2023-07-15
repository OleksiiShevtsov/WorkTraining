#pragma once

#include <boost/asio.hpp>
#include "output_notification_queue.h"

int main()
{
	boost::asio::io_context io_context;
	alexgram::OutputNotificationQueue outputNotificationQueue( io_context, 2000 );

	return 0;
} 
