#pragma once

#include "common/types.h"
#include "notifications/notification.h"

#include <string_view>

namespace sylk::Notification
{
class UserLogin : public INotification
{
public:
    UserLogin( std::string_view token );

    std::optional< common::ID > getTableId() const override;
    common::ID getPlayerId() const override;

private:
    common::ID m_playerId{};
};
} // namespace sylk::Notification
