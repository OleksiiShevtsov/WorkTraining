#pragma once

#include <iostream>
#include <array>

struct FibonacciIterator{
    bool operator !=( int num ) const {
        return num >= m_current;
    }

    FibonacciIterator& operator ++(){
        const auto tmp = m_current;
        m_current = m_current + m_last;
        m_last = tmp;
        return *this;
    }

    int operator *() const {
        return m_current;
    }

private:
    int m_current{ 1 }, m_last{ 1 };
};

struct FibonacciRange{

    explicit FibonacciRange( int max ) : m_max{ max }{}
    FibonacciIterator begin() const {
        return FibonacciIterator{};
    }

    int end() const {
        return m_max;
    }

private:
    const int m_max;
};

void fibonacciIteratorCheck(){

    for( const auto element : FibonacciRange{ 500 } ){
        printf( "%d | ", element );
    }

    std::cout << "\n---equivalent---" << std::endl;

    FibonacciRange range{ 500 };
    const auto end = range.end();
    for( auto i = range.begin(); i != end; ++i ){
        const auto& element = *i;
        printf( "%d | ", element );
    }

    std::cout << "\n---break, continue---" << std::endl;

    for( const auto element : FibonacciRange{ 500 } ){
        if( element == 8 ){ printf( "*** | " ); continue;; }
        if( element == 21 ){ printf( "*** | " ); break; }
        printf( "%d | ", element );
    }
}
