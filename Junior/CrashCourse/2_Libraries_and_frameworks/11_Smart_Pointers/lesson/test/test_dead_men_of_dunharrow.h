#pragma once

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_RUNNER

#include <catch2/catch_all.hpp>
#include <boost/scoped_array.hpp>

#include "../dead_men_of_dunharrow.h"

TEST_CASE( "ScopedPtr evaluates to" ) {
    SECTION( "true when full" ) {
        ScopedOathbreakers aragorn{ new DeadMenOfDunharrow{} };
        REQUIRE(aragorn);
    }
    SECTION("false when empty") {
        ScopedOathbreakers aragorn;
        REQUIRE_FALSE( aragorn );
    }
}

TEST_CASE( "ScopedPtr is an RAII wrapper" ) {
    REQUIRE( DeadMenOfDunharrow::oathsToFulfill == 0 );
    ScopedOathbreakers aragorn{ new DeadMenOfDunharrow{} };
    REQUIRE( DeadMenOfDunharrow::oathsToFulfill == 1 );
    {
        ScopedOathbreakers legolas{ new DeadMenOfDunharrow{} };
        REQUIRE( DeadMenOfDunharrow::oathsToFulfill == 2 );
    }
    REQUIRE( DeadMenOfDunharrow::oathsToFulfill == 1 );
}

TEST_CASE( "ScopedPtr supports pointer semantics" ) {
    auto message = "The way is shut";
    ScopedOathbreakers aragorn{ new DeadMenOfDunharrow{ message } };
    SECTION("operator*") {
        REQUIRE( (*aragorn).message == message );
    }
    SECTION("operator->") {
        REQUIRE( aragorn->message == message );
    }
    SECTION("get(), which returns a raw pointer") {
        REQUIRE( aragorn.get() != nullptr );
    }
}

TEST_CASE( "ScopedPtr supports comparison with nullptr" ) {
    SECTION( "operator==" ) {
        ScopedOathbreakers legolas{};
        REQUIRE( legolas == nullptr );
    }
    SECTION( "operator!=" ) {
        ScopedOathbreakers aragorn{ new DeadMenOfDunharrow{} };
        REQUIRE( aragorn != nullptr );
    }
}

TEST_CASE( "ScopedPtr supports swap" ) {
    auto message1 = "The way is shut.";
    auto message2 = "Until the time comes.";
    ScopedOathbreakers aragorn{ new DeadMenOfDunharrow{ message1 } };
    ScopedOathbreakers legolas{ new DeadMenOfDunharrow{ message2 } };
    aragorn.swap( legolas );
    REQUIRE( aragorn->message == message2 );
    REQUIRE( legolas->message == message1 );
}

TEST_CASE( "ScopedPtr reset" ) {
    ScopedOathbreakers aragorn{ new DeadMenOfDunharrow{} };
    SECTION( "destructs owned object" ) {
        aragorn.reset();
        REQUIRE( DeadMenOfDunharrow::oathsToFulfill == 0 );
    }
    SECTION( "destructs owned object" ) {
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

TEST_CASE( "ScopedPtr can" ) {
    ScopedOathbreakers aragorn{ new DeadMenOfDunharrow{} };
    SECTION( "be passed by reference" ) {
        byRef( aragorn );
    }
    /*SECTION( "not by copied" ) {
        // not compile
        byVal( aragorn );
        auto sonOfArathorn = aragorn;
    }
    SECTION( "not by moved" ) {
        // not compile
        byVal( std::move( aragorn ) );
        auto sonOfArathorn = std::move( aragorn );
    }*/
}

TEST_CASE( "ScopedArray supports operator[]" ) {
    boost::scoped_array<int> squares{ new int [ 5 ] { 0, 4, 9, 16, 25 } };
    squares[ 0 ] = 1;
    REQUIRE( squares[ 0 ] == 1 );
    REQUIRE( squares[ 1 ] == 4 );
    REQUIRE( squares[ 2 ] == 9 );
}
