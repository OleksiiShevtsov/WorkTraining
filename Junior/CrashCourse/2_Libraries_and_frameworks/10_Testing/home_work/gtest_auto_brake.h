#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "auto_brake/auto_brake.h"

struct AutoBrakeGTest : ::testing::Test{
    MockServiceBus bus{};
    AutoBrake autoBrake{ bus };
};

int startAllGTestsForAutoBrake(int argc, char** argv) {

    // google tests initialization
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST_F(AutoBrakeGTest, initialSpeedIsZero_G) {
    ASSERT_DOUBLE_EQ( 0, autoBrake.getSpeedMps() );
}

TEST_F( AutoBrakeGTest, initialSensitivityIsFive_G ) {
    ASSERT_DOUBLE_EQ( 5 ,autoBrake.getCollisionThresholdS() );
}

TEST_F( AutoBrakeGTest, sensitivityGreaterThenOne_G ) {
    ASSERT_ANY_THROW( autoBrake.setCollisionThresholdS( 0.5L ) );
}

TEST_F( AutoBrakeGTest, speedIsSaves_G ){
    bus.speedUpdateCallback( SpeedUpdate{ 100L } );
    ASSERT_EQ( 100L, autoBrake.getSpeedMps() );
    bus.speedUpdateCallback( SpeedUpdate{ 50L } );
    ASSERT_EQ( 50L, autoBrake.getSpeedMps() );
    bus.speedUpdateCallback( SpeedUpdate{ 0L } );
    ASSERT_DOUBLE_EQ( 0L, autoBrake.getSpeedMps() );
}

TEST_F( AutoBrakeGTest, alertWhenImminent_G ){
    autoBrake.setCollisionThresholdS( 10L );
    bus.speedUpdateCallback( SpeedUpdate{ 100L } );
    bus.carDetectedCallback( CarDetected{ 100L, 0L } );
    ASSERT_EQ( 1, bus.commandsPublished );
    ASSERT_DOUBLE_EQ( 1L, bus.lastComand.timeToCollisionS );
}

TEST_F( AutoBrakeGTest, noAlertWhenNotImminent_G ){
    autoBrake.setCollisionThresholdS( 2L );
    bus.speedUpdateCallback( SpeedUpdate{ 100L } );
    bus.carDetectedCallback( CarDetected{ 1000L, 50L } );
    ASSERT_EQ( bus.commandsPublished, 0 );
}

// home work

TEST_F( AutoBrakeGTest, lastSpeedLimit39_G ) {
    bus.speedLimitCallback( SpeedLimitDetect{ 39L } );
    ASSERT_EQ( autoBrake.speedLimitMps(), 39L );
}

TEST_F( AutoBrakeGTest, checkingTheLast3SpeedLimits_G ) {
    bus.speedLimitCallback( SpeedLimitDetect{ 100L } );
    ASSERT_EQ( 100L, autoBrake.speedLimitMps() );
    bus.speedLimitCallback( SpeedLimitDetect{ 80L } );
    ASSERT_EQ( 80L, autoBrake.speedLimitMps() );
    bus.speedLimitCallback( SpeedLimitDetect{ 60L } );
    ASSERT_EQ( 60L, autoBrake.speedLimitMps() );
}

TEST_F( AutoBrakeGTest, speedLimit35SpeedMovement34_G ) {
    bus.speedLimitCallback(SpeedLimitDetect{ 35L });
    bus.speedUpdateCallback( SpeedUpdate{ 34L } );
    ASSERT_EQ( bus.commandsPublished, 0 );
}

TEST_F( AutoBrakeGTest, speedLimit35SpeedUpdate40_G ) {
    bus.speedLimitCallback( SpeedLimitDetect{ 35L } );
    bus.speedUpdateCallback( SpeedUpdate{ 40L } );
    ASSERT_EQ( bus.commandsPublished, 1 );
}

TEST_F(AutoBrakeGTest, speedLimit35SpeedUpdate30ThenSpeedLimit25_G ) {
    bus.speedLimitCallback(SpeedLimitDetect{ 35L });
    bus.speedUpdateCallback(SpeedUpdate{ 30L });
    bus.speedLimitCallback(SpeedLimitDetect{ 25L });
    ASSERT_EQ(bus.commandsPublished, 1);
}