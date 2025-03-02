#pragma once

#include "enum.h"

#include <map>
#include <string>

namespace sylk::common::notification
{
std::string to_string( NotificationTypes ntType )
{
    static const std::map< NotificationTypes, std::string > notificationType{
        { NotificationTypes::ntMapState, "ntMapState" },
        { NotificationTypes::ntDirectionAction, "ntDirectionAction" }
    };

    return notificationType.at( ntType );
}
} // namespace sylk::common::notification
