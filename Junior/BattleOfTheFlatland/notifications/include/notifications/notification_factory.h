#pragma once

#include "notifications/custom_notifications/direction_action.h"
#include "notifications/custom_notifications/table_state.h"
#include "notifications/custom_notifications/user_login.h"
#include "notifications/notification.h"

#include <boost/core/ignore_unused.hpp>

#include <memory>
#include <optional>
#include <string>
#include <variant>

namespace bof
{
using NotificationVariant = std::variant< Notification::DirectionAction,
                                          Notification::TableState,
                                          Notification::UserLogin >;

namespace NotificationFactory
{
struct NotificationHandler
{
    std::string operator()( Notification::TableState& tableState );
    std::string operator()( INotification& notification )
    {
        boost::ignore_unused( notification );
        return {};
    };
};

std::optional< NotificationVariant > getNotificationFromProtobuf( const std::string& protoString );
std::optional< NotificationVariant >

createNotification( common::NotificationTypes notificationType, common::ID playerId );

std::string getProtoStringFromNotification( NotificationVariant notification );
} // namespace NotificationFactory
} // namespace sylk
