#include "notifications/websocket_handler.h"
#include "notifications/websocket_notification_server.h"

using namespace bof;
using namespace WebSocketHandler;

NotificationHandler::NotificationHandler( WebSocketNotificationServer& webSocketServer )
    : m_webSocketServer( webSocketServer )
{
}

void NotificationHandler::operator()( Notification::TableState& tableState )
{
    if ( tableState.getGameState() == Notification::GameState::Closed )
    {
        m_webSocketServer.sendInInputQueue( tableState );
    }

    m_webSocketServer.send( tableState.getPlayerId(), tableState );
}


