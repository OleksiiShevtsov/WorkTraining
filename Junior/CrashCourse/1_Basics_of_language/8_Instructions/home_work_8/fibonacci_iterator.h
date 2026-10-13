#pragma once

#include <iostream>

struct FibonacciIterator {
    bool operator !=( int num ) const {
        return num >= m_current;
    }

    FibonacciIterator& operator ++() {
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
