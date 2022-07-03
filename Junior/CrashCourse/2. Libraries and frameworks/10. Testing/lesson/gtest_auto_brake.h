#pragma once

#include <gtest/gtest.h>

#include "user_test/user_test_env.h"
#include "auto_brake/auto_brake.h"

struct AutoBrakeTest : ::testing::Test{
    MockServiceBus bus{};
    AutoBrake autoBrake{ bus };
};

TEST_F( AutoBrakeTest, initialSpeedIsZero) {
    ASSERT_DOUBLE_EQ( 0, autoBrake.getSpeedMps() );
}

TEST_F( AutoBrakeTest, initialSensitivityIsFive ) {
    ASSERT_DOUBLE_EQ( 5 ,autoBrake.getCollisionThresholdS() );
}

TEST_F( AutoBrakeTest, sensitivityGreaterThenOne ) {
    ASSERT_ANY_THROW( autoBrake.setCollisionThresholdS( 0.5L ) );
}

TEST_F( AutoBrakeTest, speedIsSaves ){
    bus.speedUpdateCallback( SpeedUpdate{ 100L } );
    ASSERT_EQ( 100L, autoBrake.getSpeedMps() );
    bus.speedUpdateCallback( SpeedUpdate{ 50L } );
    ASSERT_EQ( 50L, autoBrake.getSpeedMps() );
    bus.speedUpdateCallback( SpeedUpdate{ 0L } );
    ASSERT_DOUBLE_EQ( 0L, autoBrake.getSpeedMps() );
}

TEST_F( AutoBrakeTest, alertWhenImminent ){
    autoBrake.setCollisionThresholdS( 10L );
    bus.speedUpdateCallback( SpeedUpdate{ 100L } );
    bus.carDetectedCallback( CarDetected{ 100L, 0L } );
    ASSERT_EQ( 1, bus.commandsPublished );
    ASSERT_DOUBLE_EQ( 1L, bus.lastComand.m_timeToCollisionS );
}

TEST_F( AutoBrakeTest, noAlertWhenNotImminent ){
    autoBrake.setCollisionThresholdS( 2L );
    bus.speedUpdateCallback( SpeedUpdate{ 100L } );
    bus.carDetectedCallback( CarDetected{ 1000L, 50L } );
    ASSERT_EQ( bus.commandsPublished, 0 );
}

int startAllGTestsForAutoBrake( int argc, char **argv ){
    // in main
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
