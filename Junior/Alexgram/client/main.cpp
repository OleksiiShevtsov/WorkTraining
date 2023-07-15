#include <iostream>

#include "notification_client.h"

int main() 
{
    boost::asio::io_context io_context;
    NotificationClient notificationClient{ io_context, "localhost", "2000" };
    io_context.run();

    return 0;
}
