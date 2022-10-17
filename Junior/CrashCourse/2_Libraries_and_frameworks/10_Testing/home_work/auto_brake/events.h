#pragma once

struct SpeedUpdate {
    double velocityMps;
};

struct CarDetected {
    double distanceM;
    double velocityMps;
};

struct BrakeCommand {
    double timeToCollisionS;
};

struct SpeedLimitDetect {
    unsigned short speedMps;
};
