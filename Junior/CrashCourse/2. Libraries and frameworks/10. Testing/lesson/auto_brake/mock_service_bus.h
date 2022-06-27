#pragma once

#include <functional>

#include "iservice_bus.h"

struct MockServiceBus : IServiceBus {
    void publish( const BrakeCommand& cmd ) override {
        commandsPublished++;
        lastComand = cmd;
    }
    void subscribe( SpeedUpdateCallback callback ) override {
        speedUpdateCallback = callback;
    }
    void subscribe( CarDetectedCallback callback ) override {
        carDetectedCallback = callback;
    }
    BrakeCommand lastComand{};
    int commandsPublished{};
    SpeedUpdateCallback speedUpdateCallback{};
    CarDetectedCallback carDetectedCallback{};
};
