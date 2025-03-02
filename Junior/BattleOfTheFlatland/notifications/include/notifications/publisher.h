#pragma once

#include "notifications/subscriber.h"

namespace bof
{
class IPublisher
{
public:
    virtual void addSubscriber( std::shared_ptr< ISubscriber > subscriber )    = 0;
    virtual void removeSubscriber( std::shared_ptr< ISubscriber > subscriber ) = 0;
    virtual void notify( NotificationVariant notification )                    = 0;
};
} // namespace sylk
