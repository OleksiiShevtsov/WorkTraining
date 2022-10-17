#pragma once

#include <iostream>

struct PrimeNumber {
    bool operator !=( int num ) const {
        return num >= m_number;
    }

    PrimeNumber& operator ++() {
        m_number++;
        return *this;
    }

    int operator *() const {
        return m_number;
    }
private:
    int m_number{ 1 };
};
