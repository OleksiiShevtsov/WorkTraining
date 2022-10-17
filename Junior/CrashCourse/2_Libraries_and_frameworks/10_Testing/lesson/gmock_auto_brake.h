#pragma once

#include <gmock/gmock.h>

#include "gtest_auto_brake.h"

using ::testing::_;
using ::testing::A;
using ::testing::Field;
using ::testing::DoubleEq;
using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::Invoke;

// need to finish

/*struct MockServiceBus_M : IServiceBus {
    MOCK_METHOD1(publish, void(const BrakeCommand& cmd));
    MOCK_METHOD1(subscribe, void(SpeedUpdateCallback& callback));
    MOCK_METHOD1(subscribe, void(CarDetectedCallback& callback));
};*/

struct NiceAutoBrakeTest : ::testing::Test {
    NiceMock< MockServiceBus > bus;
    AutoBrake autoBrake{ bus };
};

struct StrictAutoBrakeTest : ::testing::Test {
    StrictAutoBrakeTest() {
        EXPECT_CALL( bus, subscribe( A< CarDetectedCallback >() ) )
            .Times( 1 )
            .WillOnce( Invoke( [ this ]( const auto& x ) {
                carDetectedCallback = x;
            } ) );
        EXPECT_CALL( bus, subscribe( A< SpeedUpdateCallback >() ) )
            .Times( 1 )
            .WillOnce( Invoke( [ this ]( const auto& x ) {
                speedUpdateCallback = x;
            } ) );
    }
    CarDetectedCallback carDetectedCallback;
    SpeedUpdateCallback speedUpdateCallback;
    StrictMock< MockSeviceBus > bus;
};

TEST_F( NiceAutoBrakeTest, initialSensitivityIsFive_M ) {
    ASSERT_DOUBLE_EQ( 0, autoBrake.getSpeedMps() );
}

/*TEST_F(AutoBrakeGTest, sensitivityGreaterThenOne_G) {
    ASSERT_ANY_THROW(autoBrake.setCollisionThresholdS(0.5L));
}

TEST_F(AutoBrakeGTest, speedIsSaves_G) {
    bus.speedUpdateCallback(SpeedUpdate{ 100L });
    ASSERT_EQ(100L, autoBrake.getSpeedMps());
    bus.speedUpdateCallback(SpeedUpdate{ 50L });
    ASSERT_EQ(50L, autoBrake.getSpeedMps());
    bus.speedUpdateCallback(SpeedUpdate{ 0L });
    ASSERT_DOUBLE_EQ(0L, autoBrake.getSpeedMps());
}

TEST_F(AutoBrakeGTest, alertWhenImminent_G) {
    autoBrake.setCollisionThresholdS(10L);
    bus.speedUpdateCallback(SpeedUpdate{ 100L });
    bus.carDetectedCallback(CarDetected{ 100L, 0L });
    ASSERT_EQ(1, bus.commandsPublished);
    ASSERT_DOUBLE_EQ(1L, bus.lastComand.m_timeToCollisionS);
}

TEST_F(AutoBrakeGTest, noAlertWhenNotImminent_G) {
    autoBrake.setCollisionThresholdS(2L);
    bus.speedUpdateCallback(SpeedUpdate{ 100L });
    bus.carDetectedCallback(CarDetected{ 1000L, 50L });
    ASSERT_EQ(bus.commandsPublished, 0);
}*/
