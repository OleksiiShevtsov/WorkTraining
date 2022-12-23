#pragma once

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_RUNNER

#include <catch2/catch_all.hpp>
#include <boost/math/constants/constants.hpp>
// #include <cstdlib>
#include <complex>
#include <cmath>
#include <random>
#include <limits>

TEST_CASE("math")
{
    SECTION( "std::complex has a real and imaginary component" )
    {
        std::complex< double > a{ 0.5, 14.13 };
        REQUIRE( std::real( a ) == 0.5 );
        REQUIRE( std::imag( a ) == 14.13 );
    }

    SECTION( "boost::math offers constants" )
    {
        using namespace boost::math::double_constants;
        auto sphereVolume = four_thirds_pi * std::pow( 10, 3 );
        REQUIRE( sphereVolume == Catch::Approx( 4188.7902047864 ) );
    }

    SECTION( "mt19937_64 is pseudorandom" )
    {
        std::mt19937_64 mtEngine{ 91586 };
        REQUIRE( mtEngine() == 8346843996631475880 );
        REQUIRE( mtEngine() == 2237671392849523263 );
        REQUIRE( mtEngine() == 7333164488732543658 );
    }

    SECTION( "std::random_devic is invocable" )
    {
        std::random_device rdEngine{};
        REQUIRE_NOTHROW( rdEngine() );
    }

    SECTION( "std::uniform_int_distribution produces uniform ints" )
    {
        std::mt19937_64 mtEngine{ 102787 };
        std::uniform_int_distribution< int > intDist{ 0, 10 };
        const size_t num{ 1'000'000 };
        int sum{};
        for( size_t i{}; i < num; i++ )
        {
            sum = sum + intDist( mtEngine );
        }
        const auto sampleMean = sum / double{ num };
        REQUIRE( sampleMean == Catch::Approx( 5 ).epsilon(.1) );
    }

    SECTION( "std::numeric_limits::min provides the smallest finite value." )
    {
        auto myCup = std::numeric_limits< int >::min();
        auto underfloweth = myCup - 1;
        REQUIRE( myCup < underfloweth );
    }
}