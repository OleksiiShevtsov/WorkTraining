#pragma once

#define BOOST_TEST_MODULE AutoBrakeBoostTest
#include <boost/test/included/unit_test.hpp>

#include "auto_brake/auto_brake.h"

struct AutoBrakeBoostTest {
    MockServiceBus bus{};
    AutoBrake autoBrake{ bus };
};

BOOST_FIXTURE_TEST_CASE( initialSpeedIsZero_B, AutoBrakeBoostTest ) {
    BOOST_TEST( 0 == autoBrake.getSpeedMps() );
}

BOOST_FIXTURE_TEST_CASE( initialSensitivityIsFive_B, AutoBrakeBoostTest ) {
    BOOST_TEST( 5 == autoBrake.getCollisionThresholdS() );
}

BOOST_FIXTURE_TEST_CASE( sensitivityGreaterThenOne_B, AutoBrakeBoostTest ) {
    BOOST_REQUIRE_THROW( autoBrake.setCollisionThresholdS( 0.5L ), std::exception );
}

BOOST_FIXTURE_TEST_CASE( speedIsSaves_B, AutoBrakeBoostTest ) {
    bus.speedUpdateCallback( SpeedUpdate{ 100L });
    BOOST_TEST( 100L == autoBrake.getSpeedMps() );
    bus.speedUpdateCallback( SpeedUpdate{ 50L } );
    BOOST_TEST( 50L == autoBrake.getSpeedMps() );
    bus.speedUpdateCallback( SpeedUpdate{ 0L } );
    BOOST_TEST( 0L == autoBrake.getSpeedMps() );
}

BOOST_FIXTURE_TEST_CASE( noAlertWhenNotImminent_B, AutoBrakeBoostTest ) {
    autoBrake.setCollisionThresholdS(2L);
    bus.speedUpdateCallback(SpeedUpdate{ 100L });
    bus.carDetectedCallback(CarDetected{ 1000L, 50L });
    BOOST_TEST( 0 == bus.commandsPublished );
}

BOOST_FIXTURE_TEST_CASE( alertWhenImminent_B, AutoBrakeBoostTest ) {
    autoBrake.setCollisionThresholdS( 10L );
    bus.speedUpdateCallback( SpeedUpdate{ 100L } );
    bus.carDetectedCallback( CarDetected{ 100L, 0L } );
    BOOST_TEST( 1 == bus.commandsPublished );
    BOOST_TEST( 1L == bus.lastComand.m_timeToCollisionS );
}
