#include "logic/flower.h"

flower::Flower::Flower( common::ID id, common::Coordinate x, common::Coordinate y ) :
    m_id{ id },
    m_point{ x, y }{}

flower::common::Point flower::Flower::getCoordinateFlower() const {
    return m_point;
}

flower::common::ID flower::Flower::getId() const {
    return m_id;
}
