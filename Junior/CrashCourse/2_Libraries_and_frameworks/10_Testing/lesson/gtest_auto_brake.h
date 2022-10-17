#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "auto_brake/auto_brake.h"

struct AutoBrakeGTest : ::testing::Test{
    MockServiceBus bus{};
    AutoBrake autoBrake{ bus };
};

int startAllGTestsForAutoBrake(int argc, char** argv) {

    // google gmock initialization
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    ::testing::InitGoogleMock(&argc, argv);

    // google tests initialization
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/*TEST_F(AutoBrakeGTest, initialSpeedIsZero_G) {
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
    ASSERT_DOUBLE_EQ( 1L, bus.lastComand.m_timeToCollisionS );
}

TEST_F( AutoBrakeGTest, noAlertWhenNotImminent_G ){
    autoBrake.setCollisionThresholdS( 2L );
    bus.speedUpdateCallback( SpeedUpdate{ 100L } );
    bus.carDetectedCallback( CarDetected{ 1000L, 50L } );
    ASSERT_EQ( bus.commandsPublished, 0 );
}*/
