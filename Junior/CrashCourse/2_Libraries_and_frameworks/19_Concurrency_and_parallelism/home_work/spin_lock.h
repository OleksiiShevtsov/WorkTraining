#pragma once

#include <iostream>
#include <atomic>
#include <thread>

class SpinLock {
public:

    SpinLock( const SpinLock& ) = delete;
    SpinLock operator= ( const SpinLock& ) = delete;

    SpinLock() : locked{ false }{}

    void lock() 
    {
        while ( locked );
        locked = true;
    }

    void unlock()
    {
        locked = false;
    }

private:
    bool locked;
};

// use case
SpinLock mutex;
int count = 0;

void increment() 
{
    mutex.lock();
    ++count;
    mutex.unlock();
}

void spinLockCheck() 
{
    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    std::cout << "count: " << count << std::endl;
}