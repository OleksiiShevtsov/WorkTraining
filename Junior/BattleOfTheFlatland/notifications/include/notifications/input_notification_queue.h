#pragma once

#include "notifications/publisher.h"

#include <atomic>
#include <future>
#include <queue>

namespace bof
{
class InputNotificationQueue : public IPublisher
{
public:
    InputNotificationQueue();
    ~InputNotificationQueue();

    void addSubscriber( std::shared_ptr< ISubscriber > subscriber ) override;
    void removeSubscriber( std::shared_ptr< ISubscriber > subscriber ) override;
    void notify( NotificationVariant notification ) override;

private:
    void taskProcessingLoop();

    std::mutex m_mutex;
    std::atomic< bool > m_isTerminating = false;
    std::thread m_thread;

    std::queue< NotificationVariant > m_queue;
    std::vector< std::shared_ptr< ISubscriber > > m_subscribers;
};
} // namespace sylk
