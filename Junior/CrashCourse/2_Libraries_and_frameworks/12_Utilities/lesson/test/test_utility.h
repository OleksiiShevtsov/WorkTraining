#pragma once

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_RUNNER

#include <catch2/catch_all.hpp>
#include <boost/logic/tribool.hpp>

#include "test_assistant.h"

TEST_CASE( "tribool" )
{
    using boost::logic::indeterminate;
    const boost::logic::tribool t = true, f = false, i = indeterminate;

    SECTION( "Boost tribool converts to bool" )
    {
        REQUIRE( t.value );
        REQUIRE_FALSE( f.value );
        REQUIRE( ( !f ).value );
        REQUIRE_FALSE( ( !t ).value );
        REQUIRE( indeterminate( i ) );
        REQUIRE_FALSE( indeterminate( t ) );
    }

    SECTION( "Boost tribool supports Boolean operations" )
    {
        auto tORf = t || f;
        REQUIRE( tORf.value );
        REQUIRE( indeterminate( t && indeterminate ) );
        REQUIRE( indeterminate( f || indeterminate ) );
        REQUIRE( indeterminate( !i ) );
    }

    SECTION( "Boost tribool works nicely with if statements" )
    {
        if( i )
        {
            FAIL( "indeterminate is true" );
        }
        else if ( !i )
        {
            FAIL( "indeterminate is false" );
        }
        else
        {
        }
    }
}

TEST_CASE( "optional" )
{
    SECTION( "std::optional constains types" )
    {
        if(auto matrixOpt = take( JPill::Blue ) )
        {
            REQUIRE( matrixOpt->iteration == 6 );
            auto& matrix = matrixOpt.value();
            REQUIRE( matrix.iteration == 6 );
        }
        else 
        {
            FAIL( "The optional evaluated to false" );
        }
    }

    SECTION( "std::optional can be empty" )
    {
        auto matrixOpt = take( JPill::Red );
        if( matrixOpt )
        {
            FAIL( "The matrix is not empty" );
        }
        REQUIRE_FALSE( matrixOpt.has_value() );
    }
}

TEST_CASE( "pair" )
{
    std::pair< Socialite, Valet > inimitableDuo{ bertie, reginald };

    SECTION( "std::pair permits access to members" )
    {
        REQUIRE( inimitableDuo.first.birthname == bertie.birthname );
        REQUIRE( inimitableDuo.second.surname == reginald.surname );
    }

    SECTION( "std::pair works with structured binding" )
    {
        auto& [ idleRich, butler ] = inimitableDuo;
        REQUIRE( idleRich.birthname == bertie.birthname );
        REQUIRE( butler.surname == reginald.surname );
    }
}

TEST_CASE( "tuple" )
{
    SECTION( "std::tuple permits access to members with std:get" )
    {
        using Trio = std::tuple< Socialite, Valet, Acquaintance >;
        Trio truculentTrio{ bertie, reginald, hildebrand };
        auto& bertieRef = std::get< 0 >( truculentTrio );
        REQUIRE( bertieRef.birthname == bertie.birthname );

        auto& tuppyRef = std::get< Acquaintance >( truculentTrio );
        REQUIRE( tuppyRef.nickname == hildebrand.nickname );
    }
}

TEST_CASE( "any" )
{
    SECTION( "std::any allows us to to std::any_cast into a type" )
    {
        std::any hagunemnon;
        hagunemnon.emplace< EsacapeCapsule >( 600 );
        auto capsule = std::any_cast< EsacapeCapsule >( hagunemnon );
        REQUIRE( capsule.weightKg == 600 );
        REQUIRE_THROWS_AS( 
            std::any_cast< float >( hagunemnon ), 
            std::bad_any_cast );
    }
}

TEST_CASE( "variant" )
{
    std::variant< BugblatterBeast, EsacapeCapsule > hagunemnon;

    SECTION( "std::variant" )
    {
        REQUIRE( hagunemnon.index() == 0 );

        hagunemnon.emplace< EsacapeCapsule >( 600 );
        REQUIRE( hagunemnon.index() == 1 );

        REQUIRE( std::get< EsacapeCapsule >( hagunemnon ).weightKg == 600 );
        REQUIRE( std::get< 1 >(hagunemnon).weightKg == 600);
        REQUIRE_THROWS_AS(
            std::get< 0 >( hagunemnon ),
            std::bad_variant_access );
    }

    SECTION( "std::variant" )
    {
        hagunemnon.emplace< EsacapeCapsule >( 600 );
        auto lbs = std::visit(
            []( auto& x ) { return 2.2 * x.weightKg; },
            hagunemnon);
        REQUIRE( lbs == 1320 );
    }
}
