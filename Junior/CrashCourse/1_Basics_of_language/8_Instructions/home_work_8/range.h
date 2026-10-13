#pragma once

//#include "fibonacci_iterator.h"

template < typename T >
struct Range{

    explicit Range( T *iter, int max ) : m_iter{ iter }, m_max{ max }{}

    T begin() const {
        return *m_iter;
    }

    int end() const {
        return m_max;
    }

private:
    const int m_max;
    T *m_iter;
};
