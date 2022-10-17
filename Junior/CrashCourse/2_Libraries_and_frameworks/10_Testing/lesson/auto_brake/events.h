#pragma once

struct SpeedUpdate {
    double m_velocityMps;
};

struct CarDetected {
    double m_distanceM;
    double m_velocityMps;
};

struct BrakeCommand {
    double m_timeToCollisionS;
};
