#pragma once

#define BOOST_TEST_MODULE AutoBrake
#include <boost/test/included/unit_test.hpp>

#include "auto_brake/auto_brake.h"

BOOST_AUTO_TEST_CASE( initialSpeedIsZero ) {
    MockServiceBus bus{};
    AutoBrake autoBrake{ bus };
    BOOST_TEST( 0 == autoBrake.getSpeedMps() );
}
