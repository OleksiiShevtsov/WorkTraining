#pragma once

#include <vector>
#include <memory>
#include <thread>
#include <cstdlib>
#include <time.h>

#include "common/enum.h"
#include "common/global_settings.h"
#include "common/function.h"

namespace flower {

    class Snake {

    public:

        Snake( common::ID id, common::Coordinate x, common::Coordinate y );

        std::size_t getSizeTail() const;
        common::Coordinate getCoordinateHeadX() const;
        common::Coordinate getCoordinateHeadY() const;
        common::DirectionSnake getCurrentDirectionSnake() const;
        std::vector< common::Point > getCoordinateTail() const;
        common::ID getId() const;

        void setCurrentDirectionSnake( const common::DirectionSnake currentDirectionSnake );

        void move();
        void incrementTail();
        void decrementTail();

    private:

        void movingItem( const common::Coordinate x, const common::Coordinate y );

        std::vector< common::Point > m_tail;
        common::ID m_id;
        common::DirectionSnake m_currentDirectionSnake;
    };
}
