#pragma once

namespace bof::common
{
enum NotificationTypes
{
    ntMapState,
    ntDirectionAction,

    // Server only
    ntDisconnect,
    ntReconnect
};
}
