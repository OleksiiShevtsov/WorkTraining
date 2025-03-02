#pragma once

#include "notifications/notification_server.h"
#include "notifications/subscriber.h"

#include <atomic>
#include <future>
#include <queue>

namespace bof
{
class ISubscriber;

class OutputNotificationQueue : public ISubscriber
{
public:
    OutputNotificationQueue( std::shared_ptr< INotificationServer >& server );
    ~OutputNotificationQueue();

    void update( NotificationVariant notification ) override;

private:
    void taskProcessingLoop();

    std::mutex m_mutex;
    std::atomic< bool > m_isTerminating = false;
    std::thread m_thread;

    std::queue< NotificationVariant > m_queue;

    std::shared_ptr< INotificationServer > m_notificationServer;
};
} // namespace sylk
