#pragma once

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_RUNNER

#include <catch2/catch_all.hpp>
#include <future>
#include <string>
#include <thread>

using namespace std::literals::string_literals;
using namespace std::literals::chrono_literals;

TEST_CASE( "async returns valid future" )
{
    auto theFuture = std::async( []{ return "female"s; } );
    REQUIRE( theFuture.valid() );
}

TEST_CASE( "future invalid by default" )
{
    std::future< bool > defaultFuture;
    REQUIRE_FALSE( defaultFuture.valid() );
}

TEST_CASE( "async returns the return value of the function object" )
{
    auto theFuture = std::async( []{ return "female"s; } );
    REQUIRE( theFuture.get() == "female" );
}

TEST_CASE( "get may throw" )
{
    auto ghostrider = std::async(
                []{ throw std::runtime_error{ "The pattern is full." }; } );
    REQUIRE_THROWS_AS( ghostrider.get(), std::runtime_error );
}

TEST_CASE( "wait_for indicates whether a task is ready" )
{
    auto sleepy = async( std::launch::async, []{ std::this_thread::sleep_for( 100ms ); } );
    const auto notReadyYet = sleepy.wait_for( 25ms );
    REQUIRE( notReadyYet == std::future_status::timeout );
    const auto totallyReady = sleepy.wait_for( 100ms );
    REQUIRE( totallyReady == std::future_status::ready );
}
