#pragma once

#include <future>
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

template < typename T >
class SafeQueue {
public:
    SafeQueue( const SafeQueue& ) = delete;
    SafeQueue operator=( const SafeQueue& ) = delete;

    SafeQueue(){}

    void push( T data )
    {
        std::lock_guard<std::mutex> guard( m_mutexForReadAndWrite );
        m_safeQueue.push( data );
        m_cv.notify_all();
    }

    T pop()
    {
        std::lock_guard<std::mutex> guard( m_mutexForReadAndWrite );
        if( m_safeQueue.size() > 0)
        {
            T ret = m_safeQueue.front();
            m_safeQueue.pop();
            return ret;
        }
        return 0;
    }

    T waitAndPop()
    {
        m_cv.wait( m_mutexForReadAndWrite, [ & ] { return m_safeQueue.size() > 0; } );
        T ret = m_safeQueue.front();
        m_safeQueue.pop();
        return ret;
    }

private:

    std::queue< T > m_safeQueue;
    std::mutex m_mutexForReadAndWrite;
    std::condition_variable m_cv;
};

// use case

void safeQueueCheck()
{
    SafeQueue< int > q;

    auto eatCans = std::async(std::launch::async, [&] {
        std::cout << "el: " << q.pop() << std::endl;
        } );

    q.push( 10 );
}