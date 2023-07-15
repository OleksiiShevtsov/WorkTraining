#pragma once

#include <cmath>

#include "common/enum.h"
#include "common/global_settings.h"
#include "common/types.h"

#include <chrono>

// TODO delete
#include <iostream>

namespace bof {

    // TODO

    struct Param 
    {
        float cosPfi{};
        float sinPfi{};
        float Vx{};
        float Vy{};
        float Vx0{};
        float Vy0{};
        common::Coordinate X0{};
        common::Coordinate Y0{};
        float currentTime{};
        std::chrono::milliseconds countdown{};
    };

    class DynamicObject 
    {
    public:

        DynamicObject( common::ID id, common::Point dot );

        common::ID getId() const;
        common::Point getDot() const;
        common::Angle getAngle() const;
        common::MovementState getMovementState() const;

        void setDot( common::Point dot );
        void setAngle( common::Angle alpha );
        void setMovementState( common::MovementState movementState );

        ///////////////////////////////////////////////

        const float del{ 2.0f };
        const float A{ 50.0f };
        const float Ftr{ -9.81f / 0.4f };

        Param param;
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
                    param.Vy = speedRecalculation(-A, param.Vy0);
                    m_dot.y = coordinateRecalculation(-A, param.Vy0, param.Y0);
                    if (param.Vx > del || param.Vx < -del)
                    {
                        param.Vx = speedRecalculation(Ftr * param.cosPfi, param.Vx0);
                        m_dot.x = coordinateRecalculation(Ftr * param.cosPfi, param.Vx0, param.X0);
                    }
                    break;
                case common::MovementState::DOWN: 
                    param.Vy = speedRecalculation(A, param.Vy0);
                    m_dot.y = coordinateRecalculation(A, param.Vy0, param.Y0);
                    if (param.Vx > del || param.Vx < -del)
                    {
                        param.Vx = speedRecalculation(Ftr * param.cosPfi, param.Vx0);
                        m_dot.x = coordinateRecalculation(Ftr * param.cosPfi, param.Vx0, param.X0);
                    }
                    break;
                case common::MovementState::LEFT: 
                    param.Vx = speedRecalculation( -A, param.Vx0 );
                    m_dot.x = coordinateRecalculation( -A, param.Vx0, param.X0 );
                    if ( param.Vy > del || param.Vy < -del )
                    {
                        param.Vy = speedRecalculation( Ftr * param.sinPfi, param.Vy0 );
                        m_dot.y = coordinateRecalculation( Ftr * param.sinPfi, param.Vy0, param.Y0 );
                    }
                    break;
                case common::MovementState::RIGHT: 
                    param.Vx = speedRecalculation( A, param.Vx0 );
                    m_dot.x = coordinateRecalculation( A, param.Vx0, param.X0 );
                    if ( param.Vy > del || param.Vy < -del )
                    {
                        param.Vy = speedRecalculation( Ftr * param.sinPfi, param.Vy0 );
                        m_dot.y = coordinateRecalculation( Ftr * param.sinPfi, param.Vy0, param.Y0 );
                    }
                    break;
                case common::MovementState::FREE: 
                    if ( param.Vx > del || param.Vx < -del )
                    {
                        param.Vx = speedRecalculation( Ftr * param.cosPfi, param.Vx0 );
                        m_dot.x = coordinateRecalculation( Ftr * param.cosPfi, param.Vx0, param.X0 );
                    }
                    if ( param.Vy > del || param.Vy < -del )
                    {
                        param.Vy = speedRecalculation( Ftr * param.sinPfi, param.Vy0);
                        m_dot.y = coordinateRecalculation( Ftr * param.sinPfi, param.Vy0, param.Y0);
                    }
                    break;
            };   
        }

        float speedRecalculation( float a, float V0 )
        {
            return a * param.currentTime + V0;
        }

        common::Coordinate coordinateRecalculation( float a, float V0, common::Coordinate x0 )
        {
            return a * pow(param.currentTime, 2) / 2 + V0 * param.currentTime + x0;
        }

        void getTime()
        {
            float t1 = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
            float t2 = param.countdown.count();
            param.currentTime = (t1 - t2) / 1000;
            //std::cout << param.currentTime << std::endl;
        }

        void resetEndpoints()
        {
            param.cosPfi = param.Vx / sqrt(pow(param.Vx, 2) + pow(param.Vy, 2));
            param.sinPfi = param.Vy / sqrt(pow(param.Vx, 2) + pow(param.Vy, 2));
            param.Vx0 = param.Vx;
            param.Vy0 = param.Vy;
            param.X0 = m_dot.x;
            param.Y0 = m_dot.y;
            param.currentTime = 0;
            param.countdown = std::chrono::time_point_cast <std::chrono::milliseconds> ( std::chrono::steady_clock::now() ).time_since_epoch();
        }

        ///////////////////////////////////////////////

    private:
        common::ID m_id;
        common::Point m_dot;
        common::Angle m_alpha;
        common::MovementState m_movementState;
        common::MovementState m_movementStatePrev;
    };

}
