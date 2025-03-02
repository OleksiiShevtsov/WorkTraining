#pragma once

#include "notifications/notification.h"
#include "notifications/notification_factory.h"

#include <memory>

namespace bof
{
class ISubscriber
{
public:
    virtual void update( NotificationVariant notification ) = 0;
};
} // namespace sylk
