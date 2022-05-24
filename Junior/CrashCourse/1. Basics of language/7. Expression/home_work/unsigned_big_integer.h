#pragma once

#include <cstdio>
#include <limits>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <concepts>

template <class T>
concept IntegralUnsigned = std::is_integral<T>::value; //&& std::is_unsigned<T>::value;

template< IntegralUnsigned T >
struct UnsignedBigInteger{

    UnsignedBigInteger( T value ){
        std::cout << "sizeof(T): " << sizeof(T) << std::endl;
        if( sizeof(T) > sizeDataUnsignedBigInteger ){
            throw std::overflow_error{ "overflow:" };
        }
        new ( m_dataUnsignedBigInteger ) T{ value };
    }

    constexpr T get() const {
        return *( reinterpret_cast< T* >( const_cast< char* >( m_dataUnsignedBigInteger ) ) );
    }

    UnsignedBigInteger operator+( T other ){
        UnsignedBigInteger result{ this->get() + other };
        if( result.get() < this->get() ){ throw std::overflow_error{ "overflow:" }; }
        return result;
    }

    UnsignedBigInteger operator-( T other ){
        UnsignedBigInteger result{ this->get() - other };
        if( result.get() < this->get() ){ throw std::overflow_error{ "overflow:" }; }
        return result;
    }

    UnsignedBigInteger operator*( T other ){
        UnsignedBigInteger result{ this->get() * other };
        if( result.get() < this->get() ){ throw std::overflow_error{ "overflow:" }; }
        return result;
    }

    UnsignedBigInteger operator/( T other ){
        UnsignedBigInteger result{ this->get() / other };
        if( result.get() < this->get() ){ throw std::overflow_error{ "overflow:" }; }
        return result;
    }

    UnsignedBigInteger operator%( T other ){
        UnsignedBigInteger result{ this->get() % other };
        if( result.get() < this->get() ){ throw std::overflow_error{ "overflow:" }; }
        return result;
    }

    explicit operator T() const { return get(); }

private:
    static const size_t sizeDataUnsignedBigInteger{ 16 };
    char m_dataUnsignedBigInteger[ sizeDataUnsignedBigInteger ];
};

void checkUnsignedBigInteger(){

    UnsignedBigInteger i( 10000000000000000000 );

    auto i_2 = i + 100;

    //int p = static_cast< int >( i );

    std::cout << i_2.get() << std::endl;
    //std::cout << p << std::endl;
}
