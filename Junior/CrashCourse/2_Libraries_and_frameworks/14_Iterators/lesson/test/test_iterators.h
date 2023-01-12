#pragma once

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_RUNNER

#include <catch2/catch_all.hpp>
#include <deque>
#include <list>
#include <forward_list>
#include <iterator>

#include "../movable.h"

TEST_CASE( "Insert iterators convert writes into container insertions." )
{
    std::deque< int > dq;
    auto backInsert = std::back_inserter( dq );
    *backInsert = 2; // 2
    ++backInsert;
    *backInsert = 4; // 2, 4
    ++backInsert;

    auto frontInsert = std::front_inserter( dq );
    *frontInsert = 1; // 1, 2, 4
    ++frontInsert;
    *frontInsert = 0; // 0, 1, 2, 4
    ++frontInsert;

    auto insert = std::inserter( dq, dq.begin() + 3 );
    *insert = 3; // 0, 1, 2, 3, 4
    ++insert;

    REQUIRE( dq[ 0 ] == 0 );
    REQUIRE( dq[ 1 ] == 1 );
    REQUIRE( dq[ 2 ] == 2 );
    REQUIRE( dq[ 3 ] == 3 );
    REQUIRE( dq[ 4 ] == 4 );
 }

TEST_CASE( "std::forward_list begin and end provide input iterators" )
{
    const std::forward_list< int > easyAs{ 1, 2, 3 };
    auto itr = easyAs.begin();
    REQUIRE( *itr == 1 );
    ++itr;
    REQUIRE( *itr == 2 );
    ++itr;
    REQUIRE( *itr == 3 );
    ++itr;
    REQUIRE( itr == easyAs.end() );
}

TEST_CASE( "std::forward_list's begin and end provide forward iterators" )
{
    const std::forward_list< int > easyAs{ 1, 2, 3 };
    auto itr1 = easyAs.begin();
    auto itr2{ itr1 };
    int doubleSum{};
    while( itr1 != easyAs.end() )
        doubleSum += *( itr1++ );
    while( itr2 != easyAs.end() )
        doubleSum += *( itr2++ );
    REQUIRE( doubleSum == 12 );
}

TEST_CASE( "std::list begin and end provide bidirectional iterators" )
{
    const std::vector< int > easyAs{ 1, 2, 3 };
    auto itr = easyAs.begin();
    REQUIRE( itr[ 0 ] == 1 );
    ++itr;
    REQUIRE( *( easyAs.cbegin() + 2 ) == 3 );
    REQUIRE( easyAs.cend() - itr == 2 );
}

TEST_CASE( "Mutable random-access iterators support writing." )
{
    std::deque< int > easyAs{ 1, 0, 3 };
    auto itr = easyAs.begin();
    itr[ 1 ] = 2;
    itr++;
    REQUIRE( *itr == 2 );
}

TEST_CASE( "advance modifies input iterators" )
{
    std::vector< unsigned char > mission
    {
        0x9e, 0xc4, 0xc1, 0x29,
        0x49, 0xa4, 0xf3, 0x14,
        0x74, 0xf2, 0x99, 0x05,
        0x8c, 0xe2, 0xb2, 0x2a
    };

    auto itr = mission.begin();
    std::advance( itr, 4 );
    REQUIRE( *itr == 0x49 );
    std::advance( itr, 4 );
    REQUIRE( *itr == 0x74 );
    std::advance( itr, -8 );
    REQUIRE( *itr == 0x9e );
    REQUIRE( itr == mission.begin() );
}

TEST_CASE( "next returns iterators at given offset" )
{
    std::vector< unsigned char > mission
    {
        0x9e, 0xc4, 0xc1, 0x29,
        0x49, 0xa4, 0xf3, 0x14,
        0x74, 0xf2, 0x99, 0x05,
        0x8c, 0xe2, 0xb2, 0x2a
    };

    auto itr1 = mission.begin();
    std::advance( itr1, 4 );
    REQUIRE( *itr1 == 0x49 );

    auto itr2 = std::next( itr1 );
    REQUIRE( *itr2 == 0xa4 );

    auto itr3 = std::next( itr1, 4 );
    REQUIRE( *itr3 == 0x74 );

    REQUIRE( *itr1 == 0x49 );
}

TEST_CASE( "distance returns the number of elements between iterators" )
{
    std::vector< unsigned char > mission
    {
        0x9e, 0xc4, 0xc1, 0x29,
        0x49, 0xa4, 0xf3, 0x14,
        0x74, 0xf2, 0x99, 0x05,
        0x8c, 0xe2, 0xb2, 0x2a
    };
    auto eighth = std::next( mission.begin(), 8 );
    auto fifth = std::prev( eighth, 4 );
    REQUIRE( std::distance( fifth, eighth ) == 4 );
}

TEST_CASE( "iter_swap swaps pointed-to elements" )
{
    std::vector< long > easyAs{ 3, 2, 1 };
    std::iter_swap( easyAs.begin(), std::next( easyAs.begin(), 2 ) );
    REQUIRE( easyAs[ 0 ] == 1 );
    REQUIRE( easyAs[ 1 ] == 2 );
    REQUIRE( easyAs[ 2 ] == 3 );
}

TEST_CASE( "move iterators convert accesses into move operations" )
{
    std::vector< Movable > donor;
    donor.emplace_back( 1 );
    donor.emplace_back( 2 );
    donor.emplace_back( 3 );
    std::vector< Movable > recipient
    {
        std::make_move_iterator( donor.begin() ),
        std::make_move_iterator( donor.end() ),
    };
    REQUIRE( donor[ 0 ].mId == -1 );
    REQUIRE( donor[ 1 ].mId == -1 );
    REQUIRE( donor[ 2 ].mId == -1 );
    REQUIRE( recipient[ 0 ].mId == 1 );
    REQUIRE( recipient[ 1 ].mId == 2 );
    REQUIRE( recipient[ 2 ].mId == 3 );
}

TEST_CASE( "reverse iterators can initialize containers" )
{
    std::list< int > original{ 3, 2, 1 };
    std::vector< int > easyAs{ original.crbegin(), original.crend() };
    REQUIRE( easyAs[ 0 ] == 1 );
    REQUIRE( easyAs[ 1 ] == 2 );
    REQUIRE( easyAs[ 2 ] == 3 );
}

TEST_CASE( "make_reverse_iterator converts a normal iterator" )
{
    std::list< int > original{ 3, 2, 1 };
    auto begin = std::make_reverse_iterator( original.cend() );
    auto end = std::make_reverse_iterator( original.cbegin() );
    std::vector< int > easyAs{ begin, end };
    REQUIRE( easyAs[ 0 ] == 1 );
    REQUIRE( easyAs[ 1 ] == 2 );
    REQUIRE( easyAs[ 2 ] == 3 );
}
