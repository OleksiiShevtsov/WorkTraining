#pragma once

// TODO delete
#include <iostream>

#include "logic/snake.h"
#include "logic/flower.h"

namespace flower {

    class StateObjects{
    public:

        StateObjects( const uint32_t pixelCountX, const uint32_t pixelCountY );

        common::ID getIdObject();

        std::vector< Snake > getSnakes() const;
        std::vector< Flower > getFlowers() const;
        common::SnakeSpeed getSnakeSpeed() const;
        common::ID getIdBySnakeCoordinates( const common::Coordinate x, const common::Coordinate y );

        void setSnakeSpeed( const common::SnakeSpeed snakeSpeed );

        std::chrono::milliseconds currentSnakeSpeed();
        void emergenceOfNewSnake();
        bool snakeByCoordinates( const common::Coordinate x, const common::Coordinate y );
        void trimmingSnakeById( const common::ID id );
        void eraseSnakeById( const common::ID id );
        void emergenceOfNewFlower( const common::Coordinate x, const common::Coordinate y );
        bool flowerByCoordinates( const common::Coordinate x, const common::Coordinate y );
        void removeFlowerByCoordinates( const common::Coordinate x, const common::Coordinate y  );
        void snakesGrowth();
        bool directionCheckForSnake( const common::DirectionSnake directionToCheck, const Snake snake );
        void movingSnakeToNextCoordinate();
		
		// for tests
    protected:
        std::vector< Snake > m_snakes;

    private:
        uint32_t m_pixelCountY;
        uint32_t m_pixelCountX;
        common::ID m_idObject;
        common::SnakeSpeed m_snakeSpeed;
        std::vector< Flower > m_flowers;
    };
}
