#pragma once

#include "common/types.h"
#include "notifications/notification.h"

namespace sylk::Notification
{
class BetAction : public INotification
{
public:
    BetAction( std::string_view token, common::ID tableId, int64_t betAmount );
    BetAction( common::ID playerId, common::ID tableId, int64_t betAmount );

    common::ID getPlayerId() const override;
    std::optional< common::ID > getTableId() const override;

    int64_t getBetAmount() const;

private:
    common::ID m_playerId{};
    common::ID m_tableId{};
    int64_t m_betAmount{};
};
} // namespace sylk::Notification
