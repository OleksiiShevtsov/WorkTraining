#pragma once

#include "events.h"
#include "mock_service_bus.h"

struct AutoBrake {
    AutoBrake( IServiceBus& bus ) :
        m_collisionThresholdS{ 5L },
        m_speedMps{}{
        bus.subscribe( [ this ]( const SpeedUpdate& update ){ m_speedMps = update.m_velocityMps; } );
        bus.subscribe( [ this, &bus ]( const CarDetected& cd ){
            const auto relativeVelocityMps = m_speedMps - cd.m_velocityMps;
            const auto timeTocollisionS = cd.m_distanceM / relativeVelocityMps;
            if( timeTocollisionS > 0 && timeTocollisionS <= m_collisionThresholdS ){
                bus.publish( BrakeCommand{ timeTocollisionS } );
            }
        });
    };

    void setCollisionThresholdS( double s ){
        if( s < 1 ){ throw std::invalid_argument{ "Collision less then 1." }; }
        m_collisionThresholdS = s;
    }
    double getCollisionThresholdS() const {
        return m_collisionThresholdS;
    }
    double getSpeedMps() const {
        return m_speedMps;
    }
private:
    double m_collisionThresholdS;
    double m_speedMps;
};

struct ServiceBus {
    void publish( const BrakeCommand& );
};
