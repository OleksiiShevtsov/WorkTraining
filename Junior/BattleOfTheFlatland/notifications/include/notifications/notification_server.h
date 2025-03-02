#pragma once

#include "common/types.h"
#include "notifications/notification_factory.h"

#include <websocketpp/common/connection_hdl.hpp>

#include <vector>

using websocketpp::connection_hdl;

namespace bof
{
class INotificationServer
{
public:
    virtual void start( common::ID port )                 = 0;
    virtual void stop()                                   = 0;
    virtual void send( NotificationVariant notification ) = 0;
    virtual void join()                                   = 0;
};
} // namespace sylk
