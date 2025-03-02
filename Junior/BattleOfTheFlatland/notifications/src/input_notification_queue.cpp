#include "notifications/input_notification_queue.h"

#include <algorithm>

#include <boost/core/ignore_unused.hpp>

using namespace bof;

InputNotificationQueue::InputNotificationQueue()
{
    m_thread = std::thread( &InputNotificationQueue::taskProcessingLoop, this );
}

void InputNotificationQueue::addSubscriber( std::shared_ptr< ISubscriber > subscriber )
{
    m_subscribers.push_back( subscriber );
}

void InputNotificationQueue::removeSubscriber( std::shared_ptr< ISubscriber > subscriber )
{
    boost::ignore_unused( subscriber );
}

void InputNotificationQueue::notify( NotificationVariant notification )
{
    std::lock_guard guard( m_mutex );
    m_queue.push( notification );
}

void InputNotificationQueue::taskProcessingLoop()
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
                std::for_each( m_subscribers.begin(),
                               m_subscribers.end(),
                               [ notification ]( auto& subscriber ) {
                                   subscriber->update( notification );
                               } );
                m_queue.pop();
            }
        }
    }
}

InputNotificationQueue::~InputNotificationQueue()
{
    m_isTerminating = true;
    m_thread.join();
}
