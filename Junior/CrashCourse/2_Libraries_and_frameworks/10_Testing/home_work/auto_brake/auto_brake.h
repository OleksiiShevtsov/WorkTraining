#pragma once

#include "events.h"
#include "mock_service_bus.h"

struct AutoBrake {
    AutoBrake( IServiceBus& bus ) :
        m_collisionThresholdS{ 5L },
        m_speedMps{}{
        bus.subscribe( [ this, &bus ]( const SpeedUpdate& update ){ 
            if (update.velocityMps > m_speedLimitMps) {
                bus.publish( BrakeCommand{ 0 } ); 
            }
            else { m_speedMps = update.velocityMps; } } );
        bus.subscribe( [ this, &bus ]( const CarDetected& cd ){
            const auto relativeVelocityMps = m_speedMps - cd.velocityMps;
            const auto timeTocollisionS = cd.distanceM / relativeVelocityMps;
            if( timeTocollisionS > 0 && timeTocollisionS <= m_collisionThresholdS ){
                bus.publish( BrakeCommand{ timeTocollisionS } );
            }
        });
        bus.subscribe( [ this, &bus ]( const SpeedLimitDetect& speedLimitMps ) {
            if ( m_speedMps > speedLimitMps.speedMps ) {
                bus.publish( BrakeCommand{ 0 } );
            }
            else { m_speedLimitMps = speedLimitMps.speedMps; } } );
    };

    void setCollisionThresholdS( double s ){
        if( s < 1 ){
            throw std::invalid_argument{ "Collision less then 1." };
        }
        m_collisionThresholdS = s;
    }
    double getCollisionThresholdS() const {
        return m_collisionThresholdS;
    }
    double getSpeedMps() const {
        return m_speedMps;
    }
    unsigned short speedLimitMps() const {
        return m_speedLimitMps;
    }
private:
    double m_collisionThresholdS;
    double m_speedMps;
    unsigned short m_speedLimitMps;
};
