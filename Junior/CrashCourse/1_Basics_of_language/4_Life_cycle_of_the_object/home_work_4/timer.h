#include <iostream>
#include <ctime>

struct Timer{

    Timer( const char* name )
        : m_timestamp{ time(NULL) }
        , m_movedObject{ true }
        , m_name{ name }{
        printf("constructor:\n");
    }

    Timer( const Timer& other )
        : m_timestamp{ other.m_timestamp }
        , m_movedObject{ true }
        , m_name{ other.m_name }{
        printf("copy constructor:\n");
    }

    Timer( Timer&& other ) noexcept
        : m_timestamp{ other.m_timestamp }
        , m_movedObject{ true }
        , m_name{ other.m_name }{
        other.m_timestamp = 0;
        other.m_movedObject = false;
        other.m_name = nullptr;
        printf("move constructor:\n");
    }

    Timer& operator=( const Timer& other ){
        printf("copy operator:\n");
        if(this == &other){ return *this; }
        m_timestamp = other.m_timestamp;
        m_movedObject = true;
        m_name = other.m_name;
        return *this;
    }

    Timer& operator=( Timer&& other ) noexcept {
        printf("move operator:\n");
        if(this == &other){ return *this; }
        m_timestamp = other.m_timestamp;
        m_movedObject = other.m_movedObject;
        m_name = other.m_name;
        other.m_timestamp = 0;
        other.m_movedObject = false;
        other.m_name = nullptr;
        return *this;
    }

    time_t getTimestamp() const {
        return m_timestamp;
    }

    void printTimer(){
        printf( "time: %d, name: %s\n", m_timestamp, m_name );
    }

    ~Timer(){
        m_timestamp = time( NULL ) - m_timestamp;
        if( m_movedObject ){
            printTimer();
        }
    }

private:
    time_t m_timestamp;
    bool m_movedObject;
    const char* m_name;
};
