#pragma once

#include "common/types.h"

namespace flower {

    class Flower{
    public:

        Flower( common::ID id, common::Coordinate x, common::Coordinate y );

        common::Point getCoordinateFlower() const;
        common::ID getId() const;

    private:
        common::ID m_id;
        common::Point m_point;
    };

}
