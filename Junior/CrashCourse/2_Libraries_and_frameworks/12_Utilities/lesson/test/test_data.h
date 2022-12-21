#pragma once

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_RUNNER

#include <catch2/catch_all.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <chrono>
#include <thread>

TEST_CASE( "gregorian" )
{
    SECTION( "Invalid boost:::gregorian::dates throw exceptions" )
    {
        REQUIRE_THROWS_AS(
            boost::gregorian::date( 1996, 10, 180 ),
            boost::gregorian::bad_day_of_month
        );
    }

    SECTION( "boost:::gregorian::date supports basic calendar functions" )
    {
        boost::gregorian::date d{ 1996, 10, 18 };
        REQUIRE( d.year() == 1996 );
        REQUIRE( d.month() == 10 );
        REQUIRE( d.day() == 18 );
        REQUIRE( d.day_of_year() == 292 );
        REQUIRE( d.day_of_week() == boost::date_time::Friday );
    }

    SECTION( "boost:::gregorian::date supports calendar arithmetic" )
    {
        boost::gregorian::date d1{ 1996, 10, 18 };
        boost::gregorian::date d2{ 2022, 12, 18 };
        auto duration = d2 - d1;
        REQUIRE( duration.days() == 9557 );
    }

    SECTION( "date and date_duration support addition" )
    {
        boost::gregorian::date d1{ 1996, 10, 18 };
        boost::gregorian::date_duration dur{ 9557 };
        auto d2 = d1 + dur;
        REQUIRE( d2 == boost::gregorian::from_string( "2022/12/18" ) );
    }

    SECTION( "date and date_duration support addition" )
    {
        boost::gregorian::date d1{ 1996, 10, 18 };
        boost::gregorian::date d2{ 2022, 12, 18 };
        boost::gregorian::date_period p{ d1, d2 };
        REQUIRE( p.contains(
            boost::gregorian::date{ 2008, 8, 13 } ) );
    }
}

TEST_CASE( "chrono" )
{
    using namespace std::literals::chrono_literals;

    SECTION( "std::chrono supports several clocks" )
    {
        auto sysNow = std::chrono::system_clock::now();
        auto hiresNow = std::chrono::high_resolution_clock::now();
        auto steadyNow = std::chrono::steady_clock::now();

        REQUIRE( sysNow.time_since_epoch().count() > 0 );
        REQUIRE( hiresNow.time_since_epoch().count() > 0 );
        REQUIRE( steadyNow.time_since_epoch().count() > 0 );
    }

    SECTION( "std::chrono supports several units of measurement" )
    {
        auto oneS = std::chrono::seconds( 1 );
        auto thousandMs = 1000ms;
        REQUIRE( oneS == thousandMs );
    }

    SECTION( "std::chrono supports duration_cast" )
    {
        auto billionNsAsS = std::chrono::duration_cast<
            std::chrono::seconds >( 1'000'000'000ns );
        REQUIRE( billionNsAsS.count() == 1 );
    }

    SECTION( "std::chrono used to sleep" )
    {
        auto start = std::chrono::system_clock::now();
        std::this_thread::sleep_for(100ms);
        auto end = std::chrono::system_clock::now();
        REQUIRE( end - start >= 100ms );
    }
}