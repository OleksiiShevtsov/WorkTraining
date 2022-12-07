#pragma once

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_RUNNER

#include <catch2/catch_all.hpp>
#include <boost/scoped_array.hpp>

#include "../dead_men_of_dunharrow.h"

TEST_CASE( "scoped_ptr evaluates to" )
{
    SECTION( "true when full" )
    {
        ScopedOathbreakers aragorn{ new DeadMenOfDunharrow{} };
        REQUIRE(aragorn);
    }
    SECTION( "false when empty" )
    {
        ScopedOathbreakers aragorn;
        REQUIRE_FALSE( aragorn );
    }
}

TEST_CASE( "unique_ptr evaluates to" )
{
    auto aragorn = std::make_unique<DeadMenOfDunharrow>();
    SECTION( "true when full" )
    {
        auto sonOfArartorn{ std::move( aragorn ) };
        REQUIRE( DeadMenOfDunharrow::oathsToFulfill == 1 );
    }
    SECTION( "false when empty" )
    {
        auto sonOfArartorn = std::make_unique< DeadMenOfDunharrow >();
        REQUIRE( DeadMenOfDunharrow::oathsToFulfill == 2 );
        sonOfArartorn = std::move( aragorn );
        REQUIRE( DeadMenOfDunharrow::oathsToFulfill == 1 );
    }
}

TEST_CASE( "scoped_ptr is an RAII wrapper" )
{
    REQUIRE( DeadMenOfDunharrow::oathsToFulfill == 0 );
    ScopedOathbreakers aragorn{ new DeadMenOfDunharrow{} };
    REQUIRE( DeadMenOfDunharrow::oathsToFulfill == 1 );
    {
        ScopedOathbreakers legolas{ new DeadMenOfDunharrow{} };
        REQUIRE( DeadMenOfDunharrow::oathsToFulfill == 2 );
    }
    REQUIRE( DeadMenOfDunharrow::oathsToFulfill == 1 );
}

TEST_CASE( "scoped_ptr supports pointer semantics" )
{
    auto message = "The way is shut";
    ScopedOathbreakers aragorn{ new DeadMenOfDunharrow{ message } };
    SECTION("operator*")
    {
        REQUIRE( (*aragorn).message == message );
    }
    SECTION("operator->")
    {
        REQUIRE( aragorn->message == message );
    }
    SECTION("get(), which returns a raw pointer")
    {
        REQUIRE( aragorn.get() != nullptr );
    }
}

TEST_CASE( "scoped_ptr supports comparison with nullptr" )
{
    SECTION( "operator==" )
    {
        ScopedOathbreakers legolas{};
        REQUIRE( legolas == nullptr );
    }
    SECTION( "operator!=" )
    {
        ScopedOathbreakers aragorn{ new DeadMenOfDunharrow{} };
        REQUIRE( aragorn != nullptr );
    }
}

TEST_CASE( "scoped_ptr supports swap" )
{
    auto message1 = "The way is shut.";
    auto message2 = "Until the time comes.";
    ScopedOathbreakers aragorn{ new DeadMenOfDunharrow{ message1 } };
    ScopedOathbreakers legolas{ new DeadMenOfDunharrow{ message2 } };
    aragorn.swap( legolas );
    REQUIRE( aragorn->message == message2 );
    REQUIRE( legolas->message == message1 );
}

TEST_CASE( "scoped_ptr reset" )
{
    ScopedOathbreakers aragorn{ new DeadMenOfDunharrow{} };
    SECTION( "destructs owned object" )
    {
        aragorn.reset();
        REQUIRE( DeadMenOfDunharrow::oathsToFulfill == 0 );
    }
    SECTION( "destructs owned object" )
    {
        auto message = "It was made by those who are Dead.";
        auto newDeadMen = new DeadMenOfDunharrow{ message };
        REQUIRE( DeadMenOfDunharrow::oathsToFulfill == 2 );
        aragorn.reset( newDeadMen );
        REQUIRE( DeadMenOfDunharrow::oathsToFulfill == 1 );
        REQUIRE( aragorn->message == newDeadMen->message );
        REQUIRE( aragorn.get() == newDeadMen );
    }
}

void byRef( const ScopedOathbreakers& ){}
void byVal( ScopedOathbreakers ){}

TEST_CASE( "scoped_ptr can" )
{
    ScopedOathbreakers aragorn{ new DeadMenOfDunharrow{} };
    SECTION( "be passed by reference" )
    {
        byRef( aragorn );
    }
    /*SECTION( "not by copied" )
    {
        // not compile
        byVal( aragorn );
        auto sonOfArathorn = aragorn;
    }
    SECTION( "not by moved" )
    {
        // not compile
        byVal( std::move( aragorn ) );
        auto sonOfArathorn = std::move( aragorn );
    }*/
}

TEST_CASE( "scoped_array supports operator[]" )
{
    boost::scoped_array< int > squares{ new int [ 5 ] { 0, 4, 9, 16, 25 } };
    squares[ 0 ] = 1;
    REQUIRE( squares[ 0 ] == 1 );
    REQUIRE( squares[ 1 ] == 4 );
    REQUIRE( squares[ 2 ] == 9 );
}

TEST_CASE( "unique_ptr supports operator[]" )
{
    std::unique_ptr< int[] > squares{ new int [ 5 ] { 0, 4, 9, 16, 25 } };
    squares[ 0 ] = 1;
    REQUIRE( squares[ 0 ] == 1 );
    REQUIRE( squares[ 1 ] == 4 );
    REQUIRE( squares[ 2 ] == 9 );
}

TEST_CASE( "shared_ptr can be used in copy" )
{
    auto aragorn = std::make_shared< DeadMenOfDunharrow >();
    SECTION( "true when full" )
    {
        auto sonOfArartorn{ aragorn };
        REQUIRE( DeadMenOfDunharrow::oathsToFulfill == 1 );
    }
    SECTION( "assigment" )
    {
        SharedOathbreakers sonOfArartorn;
        sonOfArartorn = aragorn;
        REQUIRE( DeadMenOfDunharrow::oathsToFulfill == 1 );
    }
    SECTION( "assigment, and original gets discarded" )
    {
        auto sonOfArartorn = std::make_shared< DeadMenOfDunharrow >();
        REQUIRE( DeadMenOfDunharrow::oathsToFulfill == 2 );
        sonOfArartorn = aragorn;
        REQUIRE( DeadMenOfDunharrow::oathsToFulfill == 1 );
    }
}

TEST_CASE( "weak_ptr lock() yields" )
{
    auto massage = "The way is shut";
    SECTION( "a shared pointer when tracked object is alive" )
    {
        auto aragorn = std::make_shared< DeadMenOfDunharrow >( massage );
        std::weak_ptr< DeadMenOfDunharrow > legolas{ aragorn };
        auto shPtr = legolas.lock();
        REQUIRE( shPtr->message == massage );
        REQUIRE( shPtr.use_count() == 2 );
    }
    SECTION( "empty when shared pointer empty" )
    {
        std::weak_ptr< DeadMenOfDunharrow > legolas;
        {
            auto aragorn = std::make_shared< DeadMenOfDunharrow >( massage );
            legolas = aragorn;
        }
        auto shPtr = legolas.lock();
        REQUIRE( shPtr == nullptr );
    }
}

TEST_CASE( "intrusive_ptr uses an embedded reference counter." )
{
    REQUIRE( refCount == 0 );
    IntrusivPtr aragorn{ new DeadMenOfDunharrow{} };
    REQUIRE( refCount == 1 );
    {
        IntrusivPtr legolas{ aragorn };
        REQUIRE( refCount == 2 );
    }
    REQUIRE( DeadMenOfDunharrow::oathsToFulfill == 1 );
}