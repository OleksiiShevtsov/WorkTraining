#include "notifications/custom_notifications/user_login.h"
#include "common/jwt_helper.hpp"

using namespace sylk;
using namespace Notification;
using namespace sylk::common;

UserLogin::UserLogin( std::string_view token )
    : m_playerId{ JwtHelper::getIdFromToken( token ) } { }

std::optional< ID > UserLogin::getTableId() const
{
    return std::nullopt;
}

ID UserLogin::getPlayerId() const
{
    return m_playerId;
}
