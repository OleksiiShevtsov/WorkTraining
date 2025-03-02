#include "notifications/custom_notifications/user_logout.h"

using namespace sylk;
using namespace Notification;
using namespace sylk::common;

UserLogout::UserLogout( ID playerId ) : m_playerId( playerId ) { }

std::optional< ID > UserLogout::getTableId() const
{
    return std::nullopt;
}

ID UserLogout::getPlayerId() const
{
    return m_playerId;
}
