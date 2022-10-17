#pragma once

#include <functional>

#include "events.h"

using SpeedUpdateCallback = std::function< void( const SpeedUpdate& ) >;
using CarDetectedCallback = std::function< void( const CarDetected& ) >;
using SpeedLimitCallback = std::function< void( const SpeedLimitDetect& ) >;

struct IServiceBus {
    virtual ~IServiceBus() = default;
    virtual void publish( const BrakeCommand& ) = 0;
    virtual void subscribe( SpeedUpdateCallback ) = 0;
    virtual void subscribe( CarDetectedCallback ) = 0;
    virtual void subscribe( SpeedLimitCallback ) = 0;
};
