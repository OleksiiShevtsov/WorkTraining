#pragma once

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_RUNNER

#include <catch2/catch_all.hpp>
#include <vector>

TEST_CASE( "std::vector supports defoult construction" )
{
    std::vector< const char* > vec;
    REQUIRE( vec.empty() );
}

TEST_CASE( "std::vector supports braced initialization" )
{
    std::vector< int > fib{ 1, 1, 2, 3, 5 };
    REQUIRE( fib[ 4 ] == 5 );
}

TEST_CASE( "std::array supports" )
{
    SECTION( "braced initialization" )
    {
        std::vector< int > fiveNine{ 5, 9 };
        REQUIRE( fiveNine[ 0 ] == 5 );
        REQUIRE( fiveNine[ 1 ] == 9 );
    }
    SECTION( "fill constructor" )
    {
        std::vector< int > fiveNines( 5, 9 );
        REQUIRE( fiveNines[ 0 ] == 9 );
        REQUIRE( fiveNines[ 4 ] == 9 );
    }
}

TEST_CASE( "std::vector supports construction from iterators" )
{
    std::array< int, 5 > fibArray{ 1, 1, 2, 3, 5 };
    std::vector< int > fibVec{ fibArray.begin(), fibArray.end() };
    REQUIRE( fibVec[ 4 ] == 5 );
    REQUIRE( fibVec.size() == fibArray.size() );
}

TEST_CASE( "std::vector assign replaces existing elements" )
{
    std::vector< int > message{ 13, 80, 110, 114, 102, 110, 101 };
    REQUIRE( message.size() == 7);
    message.assign( { 67, 97, 101, 115, 97, 114 } );
    REQUIRE( message[ 5 ] == 114 );
    REQUIRE( message.size() == 6 );
}

TEST_CASE( "std::vector insert places new elements" )
{
    std::vector< int > zeros( 3, 0 );
    auto thirdElement = zeros.begin() + 2;
    zeros.insert( thirdElement, 10 );
    REQUIRE( zeros[2] == 10 );
    REQUIRE( zeros[3] == 0 );
    REQUIRE( zeros.size() == 4 );
}

TEST_CASE( "std::vector element methods forward arguments" )
{
    std::vector< std::pair< int, int > > factors;
    factors.emplace_back( 2, 30 );
    factors.emplace_back( 3, 20 );
    factors.emplace_back( 4, 15 );
    factors.emplace( factors.begin(), 1, 60 );
    REQUIRE( factors[ 0 ].first == 1 );
    REQUIRE( factors[ 0 ].second == 60 );
}

TEST_CASE( "std::vector exposes size managment methods" )
{
    std::vector< std::array< uint8_t, 1024 > > kbStore;
    REQUIRE( kbStore.max_size() > 0 );
    REQUIRE( kbStore.empty() );

    size_t elements{ 1024 };
    kbStore.reserve( elements );
    REQUIRE( kbStore.empty() );
    REQUIRE( kbStore.capacity() == elements );

    kbStore.emplace_back();
    kbStore.emplace_back();
    kbStore.emplace_back();
    REQUIRE( kbStore.size() == 3 );

    kbStore.shrink_to_fit();
    REQUIRE( kbStore.capacity() >= 3 );

    kbStore.clear();
    REQUIRE( kbStore.empty() );
    REQUIRE( kbStore.capacity() >= 3 );
}