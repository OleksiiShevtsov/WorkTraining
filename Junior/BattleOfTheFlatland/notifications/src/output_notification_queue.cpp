#include "notifications/output_notification_queue.h"

using namespace bof;

OutputNotificationQueue::OutputNotificationQueue( std::shared_ptr< INotificationServer >& server )
    : m_notificationServer( server )
{
    m_thread = std::thread( &OutputNotificationQueue::taskProcessingLoop, this );
}

void OutputNotificationQueue::update( NotificationVariant notification )
{
    std::lock_guard guard( m_mutex );
    m_queue.push( notification );
}

void OutputNotificationQueue::taskProcessingLoop()
{
    while ( !m_isTerminating )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 150 ) );
        if ( !m_queue.empty() )
        {
            std::lock_guard guard( m_mutex );
            for ( size_t i = 0; i < m_queue.size(); i++ )
            {
                const auto notification = m_queue.front();
                m_notificationServer->send( notification );
                m_queue.pop();
            }
        }
    }
}

OutputNotificationQueue::~OutputNotificationQueue()
{
    m_isTerminating = true;
    m_thread.join();
}
