#include "notifications/custom_notifications/direction_action.h"

using namespace bof;
using namespace bof::Notification;
using namespace bof::common;

DirectionAction::DirectionAction( ID playerId, ID tableId, int64_t betAmount )
    : m_playerId{ playerId }
    , m_tableId{ tableId }
{
}

ID DirectionAction::getPlayerId() const
{
    return m_playerId;
}

std::optional< ID > DirectionAction::getTableId() const
{
    return m_tableId;
}
