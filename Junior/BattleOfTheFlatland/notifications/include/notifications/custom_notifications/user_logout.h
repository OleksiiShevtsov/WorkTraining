#pragma once

#include "common/types.h"
#include "notifications/notification.h"

namespace bof::Notification
{
class UserLogout : public INotification
{
public:
    UserLogout( common::ID playerID );

    std::optional< common::ID > getTableId() const override;
    common::ID getPlayerId() const override;

private:
    common::ID m_playerId{};
};
} // namespace sylk::Notification
