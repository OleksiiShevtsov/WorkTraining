#pragma once

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_RUNNER

#include <catch2/catch_all.hpp>
#include <array>

TEST_CASE("std::array")
{
    std::array< int, 10 > staticArray{};
    REQUIRE( staticArray[ 0 ] == 0 );

    SECTION( "uninitialized without braced initializers" )
    {
        std::array< int, 10 > localArray;
        REQUIRE( localArray[ 0 ] != 0 );
    }
    SECTION( "initialized with braced initializers" )
    {
        std::array< int, 10 > localArray{ 1, 2, 3, 4 };
        REQUIRE( localArray[ 0 ] == 1 );
        REQUIRE( localArray[ 1 ] == 2 );
        REQUIRE( localArray[ 2 ] == 3 );
        REQUIRE( localArray[ 3 ] == 4 );
        REQUIRE( localArray[ 4 ] == 0 );
    }
}

TEST_CASE( "std::array access" )
{
    std::array< int, 4 > fib{ 1, 1, 0, 3 };

    SECTION( "operator[] cen get and set elements" )
    {
        fib[ 2 ] = 2;
        REQUIRE( fib[ 2 ] == 2 );
        // fib[ 4 ] = 5; error!
    }

    SECTION( "at() can get and set elements" )
    {
        fib.at( 2 ) = 2;
        REQUIRE( fib.at( 2 ) == 2 );
        REQUIRE_THROWS_AS( fib.at( 4 ), std::out_of_range );
    }

    SECTION( "get can get and set elements" )
    {
        std::get< 2 >( fib ) = 2;
        REQUIRE( std::get< 2 >( fib ) == 2 );
        // std::get< 4 >( fib ); error!
    }
}

TEST_CASE( "std::array has convenience methods" )
{
    std::array< int, 4 > fib{ 0, 1, 2, 0 };

    SECTION( "front" )
    {
        fib.front() = 1;
        REQUIRE( fib.front() == 1 );
        REQUIRE( fib.front() == fib[ 0 ] );
    }

    SECTION( "back" )
    {
        fib.back() = 3;
        REQUIRE( fib.back() == 3 );
        REQUIRE( fib.back() == fib[ 3 ] );
    }
}

TEST_CASE( "We can obtain a pointer to the first element using" )
{
    std::array< char, 9 > color{
        'o', 'c', 't', 'a', 'r', 'i', 'n', 'e' };
    const auto* colorPtr = color.data();

    SECTION( "data" )
    {
        REQUIRE( *colorPtr == 'o' );
    }

    SECTION( "address-of front" )
    {
        REQUIRE( &color.front() == colorPtr );
    }

    SECTION( "address-of at( 0 )" )
    {
        REQUIRE( &color.at( 0 ) == colorPtr );
    }

    SECTION( "address-of [ 0 ]" )
    {
        REQUIRE( &color[ 0 ] == colorPtr );
    }
}

TEST_CASE( "std::array bagin/end from a half-open range" )
{
    std::array< int, 0 > e{};
    REQUIRE( e.begin() == e.end() );
}

TEST_CASE( "std::array iterators are pointer-like" )
{
    std::array< int, 3 > easyAs{ 1, 2, 3 };
    auto iter = easyAs.begin();
    REQUIRE( *iter == 1 );
    ++iter;
    REQUIRE( *iter == 2 );
    ++iter;
    REQUIRE( *iter == 3 );
    ++iter;
    REQUIRE( iter == easyAs.end() );
}

TEST_CASE( "std::array can be used as a range expression" )
{
    std::array< int, 5 > fib{ 1, 1, 2, 3, 5 };
    int sum{};
    for( const auto element : fib )
        sum += element;
    REQUIRE( sum == 12 );
}