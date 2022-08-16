#pragma once

#include "user_test/user_test_env.h"
#include "auto_brake/auto_brake.h"

void initialSpeedIsZero() {
    MockServiceBus bus{};
    AutoBrake autoBrake{ bus };
    assertThat( autoBrake.getSpeedMps() == 0L, "speed not equal 0" );
}

void initialSensitivityIsFive() {
    MockServiceBus bus{};
    AutoBrake autoBrake{ bus };
    assertThat( autoBrake.getCollisionThresholdS() == 5L, "sensitivity is not 5" );
}

void sensitivityGreaterThenOne() {
    MockServiceBus bus{};
    AutoBrake autoBrake{ bus };
    try {
        autoBrake.setCollisionThresholdS( 0.5L );
    }
    catch ( const std::exception& ) {
        return;
    }
    assertThat( false, "no exception thrown" );
}

void speedIsSaves(){
    MockServiceBus bus{};
    AutoBrake autoBrake{ bus };
    bus.speedUpdateCallback( SpeedUpdate{ 100L } );
    assertThat( 100L == autoBrake.getSpeedMps(), "speed not saved to 100" );
    bus.speedUpdateCallback( SpeedUpdate{ 50L } );
    assertThat( 50L == autoBrake.getSpeedMps(), "speed not saved to 50" );
    bus.speedUpdateCallback( SpeedUpdate{ 0L } );
    assertThat( 0L == autoBrake.getSpeedMps(), "speed not saved to 0" );
}

void alertWhenImminent(){
    MockServiceBus bus{};
    AutoBrake autoBrake{ bus };
    autoBrake.setCollisionThresholdS( 10L );
    bus.speedUpdateCallback( SpeedUpdate{ 100L } );
    bus.carDetectedCallback( CarDetected{ 100L, 0L } );
    assertThat( bus.commandsPublished == 1, "1 brake commands published" );
    assertThat( bus.lastComand.m_timeToCollisionS == 1L, "time to collision not computed correctly." );
}

void noAlertWhenNotImminent(){
    MockServiceBus bus{};
    AutoBrake autoBrake{ bus };
    autoBrake.setCollisionThresholdS( 2L );
    bus.speedUpdateCallback( SpeedUpdate{ 100L } );
    bus.carDetectedCallback( CarDetected{ 1000L, 50L } );
    assertThat( bus.commandsPublished == 0, "brake command were published" );
}

void startAllTestsForAutoBrake(){
    runUserTest( initialSpeedIsZero, "initial speed is 0" );
    runUserTest( initialSensitivityIsFive, "initial sensitivity is 5" );
    runUserTest( sensitivityGreaterThenOne, "sensitivity greater then 1" );
    runUserTest( speedIsSaves, "speed is saves" );
    runUserTest( alertWhenImminent, "alert when imminent" );
    runUserTest( noAlertWhenNotImminent, "no alert when not imminent" );
}
