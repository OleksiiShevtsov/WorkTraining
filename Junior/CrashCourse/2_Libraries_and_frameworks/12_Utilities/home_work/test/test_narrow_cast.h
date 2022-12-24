#pragma once

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_RUNNER

#include <catch2/catch_all.hpp>
#include <limits>

#include "../narrow_cast.h"

TEST_CASE("narrow_cast")
{
    SECTION("boost::boost::numeric_cast checks overflow")
    {
        auto yugs = std::numeric_limits< double >::max();
        REQUIRE_FALSE( narrowCast< int >(yugs) );
    }
}
