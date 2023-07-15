#include "notifications/custom_notifications/bet_action.h"
#include "common/jwt_helper.hpp"

using namespace sylk;
using namespace sylk::Notification;
using namespace sylk::common;

BetAction::BetAction( std::string_view token, ID tableId, int64_t betAmount )
    : m_playerId{ JwtHelper::getIdFromToken( token ) }
    , m_tableId{ tableId }
    , m_betAmount{ betAmount }
{
}

BetAction::BetAction( ID playerId, ID tableId, int64_t betAmount )
    : m_playerId{ playerId }
    , m_tableId{ tableId }
    , m_betAmount{ betAmount }
{
}

ID BetAction::getPlayerId() const
{
    return m_playerId;
}

std::optional< ID > BetAction::getTableId() const
{
    return m_tableId;
}

int64_t BetAction::getBetAmount() const
{
    return m_betAmount;
}
