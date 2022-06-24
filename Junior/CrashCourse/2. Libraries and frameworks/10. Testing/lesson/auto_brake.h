#pragma once

struct SpeedUpdate {
    double velocityMps;
};

struct CarDetect {
    double distanceM;
    double velocityMps;
};

struct BrakeCommand {
    double timeToCollisionS;
};

struct ServiceBus {
    void publish( const BrakeCommand& );
};

template < typename T >
struct AutoBrake {
    AutoBrake( const T& publish );
    void observe( const SpeedUpdate& );
    void observe( const CarDetect& );
private:
    const T& m_publish;
};
