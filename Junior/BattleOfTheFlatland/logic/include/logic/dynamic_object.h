#pragma once

#include <cmath>

#include "common/enum.h"
#include "common/global_settings.h"
#include "common/types.h"

#include <chrono>

namespace bof {

    struct DynamicDotMovementParameters
    {
        float cosAcceleration{};
        float sinAcceleration{};
        float speedX{};
        float speedY{};
        float initialSpeedX{};
        float initialSpeedY{};
        common::Coordinate initialCoordinateX{};
        common::Coordinate initialCoordinateY{};
        float currentTime{};
        std::chrono::milliseconds countdown{};
    };

    class DynamicObject
    {
    public:

        DynamicObject(common::ID id, common::Point dot, float speedLimit);

        common::ID getId() const;
        common::Point getDot() const;
        common::Angle getAngle() const;
        common::MovementState getMovementState() const;

        void setDot( common::Point dot );
        void setAngle( common::Angle alpha );
        void setMovementState( common::MovementState movementState );

        // TODO 
        const float del{ 2.0f };
        const float A{ 50.0f };
        const float Ftr{ -9.81f / 0.4f };

        void recalculationDot()
        {
            if (m_movementState != m_movementStatePrev)
            {
                resetEndpoints();
                m_movementStatePrev = m_movementState;
            }
            getTime();

            switch ( m_movementState )
            {
                case common::MovementState::UP:
                    m_movementParameters.speedY = speedRecalculation(-A, m_movementParameters.initialSpeedY);
                    m_dot.y = coordinateRecalculation(-A, m_movementParameters.initialSpeedY, m_movementParameters.initialCoordinateY);
                    if (m_movementParameters.speedX > del || m_movementParameters.speedY < -del)
                    {
                        m_movementParameters.speedX = speedRecalculation(Ftr * m_movementParameters.cosAcceleration, m_movementParameters.initialSpeedX);
                        m_dot.x = coordinateRecalculation(Ftr * m_movementParameters.cosAcceleration, m_movementParameters.initialSpeedX, m_movementParameters.initialCoordinateX);
                    }
                    break;
                case common::MovementState::DOWN:
                    m_movementParameters.speedY = speedRecalculation(A, m_movementParameters.initialSpeedY);
                    m_dot.y = coordinateRecalculation(A, m_movementParameters.initialSpeedY, m_movementParameters.initialCoordinateY);
                    if (m_movementParameters.speedX > del || m_movementParameters.speedX < -del)
                    {
                        m_movementParameters.speedX = speedRecalculation(Ftr * m_movementParameters.cosAcceleration, m_movementParameters.initialSpeedX);
                        m_dot.x = coordinateRecalculation(Ftr * m_movementParameters.cosAcceleration, m_movementParameters.initialSpeedX, m_movementParameters.initialCoordinateX);
                    }
                    break;
                case common::MovementState::LEFT:
                    m_movementParameters.speedX = speedRecalculation( -A, m_movementParameters.initialSpeedX);
                    m_dot.x = coordinateRecalculation( -A, m_movementParameters.initialSpeedX, m_movementParameters.initialCoordinateX);
                    if (m_movementParameters.speedY > del || m_movementParameters.speedY < -del )
                    {
                        m_movementParameters.speedY = speedRecalculation( Ftr * m_movementParameters.sinAcceleration, m_movementParameters.initialSpeedY);
                        m_dot.y = coordinateRecalculation( Ftr * m_movementParameters.sinAcceleration, m_movementParameters.initialSpeedY, m_movementParameters.initialCoordinateY);
                    }
                    break;
                case common::MovementState::RIGHT:
                    m_movementParameters.speedX = speedRecalculation( A, m_movementParameters.initialSpeedX);
                    m_dot.x = coordinateRecalculation( A, m_movementParameters.initialSpeedX, m_movementParameters.initialCoordinateX);
                    if (m_movementParameters.speedY > del || m_movementParameters.speedY < -del )
                    {
                        m_movementParameters.speedY = speedRecalculation( Ftr * m_movementParameters.sinAcceleration, m_movementParameters.initialSpeedY);
                        m_dot.y = coordinateRecalculation( Ftr * m_movementParameters.sinAcceleration, m_movementParameters.initialSpeedY, m_movementParameters.initialCoordinateY);
                    }
                    break;
                case common::MovementState::FREE:
                    if (m_movementParameters.speedX > del || m_movementParameters.speedX < -del )
                    {

                        m_movementParameters.speedX = speedRecalculation( Ftr * m_movementParameters.cosAcceleration, m_movementParameters.initialSpeedX);
                        m_dot.x = coordinateRecalculation( Ftr * m_movementParameters.cosAcceleration, m_movementParameters.initialSpeedX, m_movementParameters.initialCoordinateX);
                    }
                    if (m_movementParameters.speedY > del || m_movementParameters.speedY < -del )
                    {
                        m_movementParameters.speedY = speedRecalculation( Ftr * m_movementParameters.sinAcceleration, m_movementParameters.initialSpeedY);
                        m_dot.y = coordinateRecalculation( Ftr * m_movementParameters.sinAcceleration, m_movementParameters.initialSpeedY, m_movementParameters.initialCoordinateY);
                    }
                    break;
            };   
        }

    private:

        float speedRecalculation( float acceleration, float initialSpeed )
        {
            //float speed = acceleration * m_movementParameters.currentTime + initialSpeed;
            //if(speed > m_speedLimitm) {
            //    return m_speedLimitm;
            //}
            return acceleration * m_movementParameters.currentTime + initialSpeed;
        }

        common::Coordinate coordinateRecalculation( float acceleration, float initialSpeed, common::Coordinate initialCoordinate)
        {
            return acceleration * pow(m_movementParameters.currentTime, 2) / 2 + initialSpeed * m_movementParameters.currentTime + initialCoordinate;
        }

        void getTime()
        {
            float t1 = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
            float t2 = m_movementParameters.countdown.count();
            m_movementParameters.currentTime = (t1 - t2) / 1000;
        }

        void resetEndpoints()
        {
            m_movementParameters.cosAcceleration = m_movementParameters.speedX / sqrt(pow(m_movementParameters.speedX, 2) + pow(m_movementParameters.speedY, 2));
            m_movementParameters.sinAcceleration = m_movementParameters.speedY / sqrt(pow(m_movementParameters.speedX, 2) + pow(m_movementParameters.speedY, 2));
            m_movementParameters.initialSpeedX = m_movementParameters.speedX;
            m_movementParameters.initialSpeedY = m_movementParameters.speedY;
            m_movementParameters.initialCoordinateX = m_dot.x;
            m_movementParameters.initialCoordinateY = m_dot.y;
            m_movementParameters.currentTime = 0;
            m_movementParameters.countdown = std::chrono::time_point_cast <std::chrono::milliseconds> (std::chrono::steady_clock::now()).time_since_epoch();
        }

    private:
        common::ID m_id;
        common::Point m_dot;
        common::Angle m_alpha;
        float m_speedLimitm;
        common::MovementState m_movementState;
        common::MovementState m_movementStatePrev;
        DynamicDotMovementParameters m_movementParameters;
    };
}
