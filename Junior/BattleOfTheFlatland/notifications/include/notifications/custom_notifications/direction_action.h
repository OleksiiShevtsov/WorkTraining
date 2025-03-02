#pragma once

#include "common/types.h"
#include "notifications/notification.h"

namespace bof::Notification
{
class DirectionAction : public INotification
{
public:
    DirectionAction( common::ID playerId, common::ID tableId, int64_t betAmount );

    common::ID getPlayerId() const override;
    std::optional< common::ID > getTableId() const override;

private:
    common::ID m_playerId{};
    common::ID m_tableId{};
};
} // namespace sylk::Notification
