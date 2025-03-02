#pragma once

#include "common/types.h"
#include "notifications/input_notification_queue.h"
#include "notifications/notification_server.h"

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/endpoint.hpp>
#include <websocketpp/roles/server_endpoint.hpp>
#include <websocketpp/server.hpp>

#include <set>
#include <thread>

using websocketpp::connection_hdl;

namespace bof
{
typedef websocketpp::server< websocketpp::config::asio > server;
using IdPerConnection =
    std::map< common::ID, std::shared_ptr< websocketpp::connection< websocketpp::config::asio > > >;

class WebSocketNotificationServer : public INotificationServer
{
public:
    WebSocketNotificationServer( std::shared_ptr< InputNotificationQueue >& inputQueue );

    void start( common::ID port ) override;
    void stop() override;
    void join() override;

    void send( NotificationVariant notification ) override;
    void send( common::ID playerId, const NotificationVariant& notification );

    // For handler
    void sendInInputQueue( const NotificationVariant& notification ) const;

    std::vector< common::ID > getAllIdConnections() const;

private:
    void send( connection_hdl hdl, const std::string& msg ) const;
    void onClose( connection_hdl hdl );
    void onOpen( connection_hdl hdl );
    void msgHandler( connection_hdl hdl, server::message_ptr msg );
    void checkConnection( common::ID playerId );

    void
    loginHandler( common::ID playerId, const connection_hdl& hdl, const std::string& binaryMsg );
    void logoutHandler( common::ID playerId );

    std::thread m_thread;
    std::shared_ptr< server > m_server;

    std::shared_ptr< IPublisher > m_inputQueue;
    IdPerConnection m_playerIdPerConnection;
    std::set< common::ID > m_disconnectedPlayerIds;
};
} // namespace sylk
