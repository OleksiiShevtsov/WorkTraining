#pragma once

template <typename T>
struct SimpleUniquePointer{

    SimpleUniquePointer() = default;

    SimpleUniquePointer( T* pointer )
        : m_pointer{ pointer }{
    }

    SimpleUniquePointer( const SimpleUniquePointer& other ) = delete;
    SimpleUniquePointer& operator=( const SimpleUniquePointer& other ) = delete;

    SimpleUniquePointer( SimpleUniquePointer&& other ) noexcept
        : m_pointer{ other.m_pointer }{
        other.m_pointer = nullptr;
    }

    SimpleUniquePointer& operator=( SimpleUniquePointer&& other ) noexcept {
        if( m_pointer ) delete m_pointer;
        m_pointer = other.m_pointer;
        other.m_pointer = nullptr;
        return *this;
    }

    ~SimpleUniquePointer(){
        if( m_pointer ) delete m_pointer;
    }

    T* getPointer() const {
        return m_pointer;
    }
private:
    T* m_pointer;
};
