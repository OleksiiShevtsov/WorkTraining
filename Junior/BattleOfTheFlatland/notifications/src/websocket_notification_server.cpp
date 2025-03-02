#include "notifications/websocket_notification_server.h"
#include "common/global_settings.h"
#include "common/notification/enum.h"
#include "common/notification/to_string.h"
#include "notifications/custom_notifications/user_login.h"
#include "notifications/notification_factory.h"
#include "notifications/websocket_handler.h"

#include <boost/core/ignore_unused.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/copy.hpp>

//#include <g3log/g3log.hpp>

using namespace bof;
using namespace bof::common;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

WebSocketNotificationServer::WebSocketNotificationServer(
    std::shared_ptr< InputNotificationQueue >& inputQueue )
    : m_server{ std::make_shared< server >() }
    , m_inputQueue{ inputQueue }
{
    m_server->set_open_handler( std::bind( &WebSocketNotificationServer::onOpen, this, ::_1 ) );
    m_server->set_close_handler( std::bind( &WebSocketNotificationServer::onClose, this, ::_1 ) );
    m_server->set_message_handler(
        std::bind( &WebSocketNotificationServer::msgHandler, this, ::_1, ::_2 ) );
    m_server->clear_access_channels( websocketpp::log::alevel::frame_header
                                     | websocketpp::log::alevel::frame_payload );
}

void WebSocketNotificationServer::start( ID port )
{
    //LOG( INFO ) << "Server start listening port: " << port;

    m_server->init_asio();
    m_server->set_reuse_addr( true );
    m_server->listen( port );
    m_server->start_accept();

    m_thread = std::thread( &server::run, m_server );
}

void WebSocketNotificationServer::stop()
{
    m_server->stop_listening();
    m_server->stop();
}

void WebSocketNotificationServer::join()
{
    m_thread.join();
}

void WebSocketNotificationServer::send( connection_hdl hdl, const std::string& msg ) const
{
    websocketpp::lib::error_code ec;
    m_server->send( hdl, msg, websocketpp::frame::opcode::BINARY, ec );
    if ( ec )
    {
        //LOG( WARNING ) << "Failed to send: " << ec.message();
    }
}

void WebSocketNotificationServer::send( NotificationVariant notification )
{
    std::visit( WebSocketHandler::NotificationHandler{ *this }, notification );
}

void WebSocketNotificationServer::sendInInputQueue( const NotificationVariant& notification ) const
{
    m_inputQueue->notify( notification );
}

void WebSocketNotificationServer::send( ID playerId, const NotificationVariant& notification )
{
    const auto connection{ m_playerIdPerConnection.find( playerId ) };
    if ( connection != m_playerIdPerConnection.end()
         && !connection->second->get_handle().expired() )
    {
        websocketpp::lib::error_code ec;
        m_server->send( connection->second->get_handle(),
                        NotificationFactory::getProtoStringFromNotification( notification ),
                        websocketpp::frame::opcode::BINARY,
                        ec );

        if ( ec )
        {
            if ( m_disconnectedPlayerIds.contains( playerId ) )
            {
                //LOG( INFO ) << "Player's id already in disconnected container";
                return;
            }

            //LOG( INFO ) << "Failed to send: " << ec.message()
            //            << "; Storing player with id: " << playerId << " to disconnected state...";

            m_disconnectedPlayerIds.emplace( playerId );
            m_playerIdPerConnection.erase( playerId );

            const auto disconnectionNotification =
                NotificationFactory::createNotification( NotificationTypes::ntDisconnect,
                                                         playerId );
            if ( disconnectionNotification )
            {
                sendInInputQueue( disconnectionNotification.value() );
            }
        }
        else
        {
            m_server->pong( connection->second->get_handle(), "Pong" );
            m_server->set_pong_timeout( GlobalSettings::replyTimeoutDuration );
        }
    }
}

std::vector< ID > WebSocketNotificationServer::getAllIdConnections() const
{
    std::vector< ID > allId;
    boost::copy( m_playerIdPerConnection | boost::adaptors::map_keys, std::back_inserter( allId ) );
    return allId;
}

void WebSocketNotificationServer::onOpen( connection_hdl hdl )
{
    m_server->pong( hdl, "Pong" );
    m_server->set_pong_timeout( GlobalSettings::replyTimeoutDuration );
}

void WebSocketNotificationServer::onClose( connection_hdl hdl )
{
    boost::ignore_unused( hdl );
}

void WebSocketNotificationServer::msgHandler( connection_hdl hdl, server::message_ptr msg )
{
    const auto connection = m_server->get_con_from_hdl( hdl );
    //LOG( INFO ) << "Received message from " << connection->get_host();
    const auto msgBinaryString = msg->get_payload();

    // WARNING: This part is important.
    // Each new .proto message should have nt_type field as a first one so
    // NotificationFactory can recognize the type of notification.

    NotificationTypeMoc typeMoc;
    typeMoc.ParseFromString( msgBinaryString );
    const auto type{ static_cast< NotificationTypes >( typeMoc.nt_type() ) };
    const auto playerId{ JwtHelper::getIdFromToken( typeMoc.token() ) };

    //LOG( INFO ) << "Notification: "
    //            << notification::to_string( static_cast< NotificationTypes >( type ) )
    //            << " was received by client " << connection->get_host();

    // IMPORTANT: UserLogin notification should firstly pass to the
    // input queue so server can send pools back to the client.
    // It's all about how pools list notification implemented,
    // maybe it should be redone.

    switch ( type )
    {
        case NotificationTypes::ntUserLoginToServer:
        {
            loginHandler( playerId, hdl, msgBinaryString );
            break;
        }
        case NotificationTypes::ntLogoutFromServer:
        {
            logoutHandler( playerId );
            return;
        }
        default:
        {
        }
    }

    if ( !m_playerIdPerConnection.contains( playerId ) )
    {
        //LOG( WARNING ) << "Cannot process notification to unauthorized user";
        return;
    }

    sendInInputQueue( NotificationFactory::getNotificationFromProtobuf( msgBinaryString ).value() );
}

void WebSocketNotificationServer::checkConnection( ID playerId )
{
    const auto connection{ m_playerIdPerConnection.find( playerId ) };
    if ( connection != m_playerIdPerConnection.end()
         && !connection->second->get_handle().expired() )
    {
        websocketpp::lib::error_code ec;
        server::message_ptr msg;
        m_server->send( connection->second->get_handle(), msg, ec );
        if ( ec )
        {
            if ( m_disconnectedPlayerIds.contains( playerId ) )
            {
                return;
            }

            m_disconnectedPlayerIds.emplace( playerId );
            m_playerIdPerConnection.erase( playerId );
        }
    }
}

void WebSocketNotificationServer::loginHandler( ID playerId,
                                                const connection_hdl& hdl,
                                                const std::string& binaryMsg )
{
    checkConnection( playerId );
    if ( m_disconnectedPlayerIds.contains( playerId ) )
    {
        m_playerIdPerConnection.emplace( playerId, m_server->get_con_from_hdl( hdl ) );
        m_disconnectedPlayerIds.erase( playerId );
        //LOG( INFO ) << "User with id: " << playerId << " back from disconnect state";

        const auto stopDisconnectionNotification =
            NotificationFactory::createNotification( NotificationTypes::ntReconnect, playerId );
        if ( stopDisconnectionNotification )
        {
            sendInInputQueue( stopDisconnectionNotification.value() );
            // Normal sending of notification farther.
            sendInInputQueue(
                NotificationFactory::getNotificationFromProtobuf( binaryMsg ).value() );
        }

        return;
    }

    const auto playerIterator = m_playerIdPerConnection.find( playerId );
    if ( playerIterator == m_playerIdPerConnection.end() )
    {
        m_playerIdPerConnection.emplace( playerId, m_server->get_con_from_hdl( hdl ) );
        //LOG( INFO ) << "User authorized by id: " << playerId;
    }
    else
    {
        //LOG( INFO ) << "User with id: " << playerId << " already authorized";
    }
}

void WebSocketNotificationServer::logoutHandler( ID playerId )
{
    const auto playerIt = m_playerIdPerConnection.find( playerId );
    if ( playerIt != m_playerIdPerConnection.end() )
    {
        //LOG( INFO ) << "Logout player with id: " << playerId;
        m_playerIdPerConnection.erase( playerIt );
        if ( m_disconnectedPlayerIds.contains( playerId ) )
        {
            m_disconnectedPlayerIds.erase( playerId );
        }
    }
    else
    {
        //LOG( WARNING ) << "Cannot erase player because we have no connection with it, id: "
        //               << playerId;
    }
}
