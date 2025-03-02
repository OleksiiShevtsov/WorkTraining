#pragma once

#include "notifications/notification_factory.h"

#include <boost/core/ignore_unused.hpp>

namespace bof
{
class WebSocketNotificationServer;
namespace WebSocketHandler
{
struct NotificationHandler
{
public:
    NotificationHandler( WebSocketNotificationServer& webSocketServer );

    void operator()( Notification::TableState& tableState );
    void operator()( INotification& nt ) { boost::ignore_unused( nt ); };

private:
    WebSocketNotificationServer& m_webSocketServer;
};
} // namespace WebSocketHandler
} // namespace sylk
