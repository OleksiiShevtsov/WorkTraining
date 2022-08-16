#include "logic/snake.h"

flower::Snake::Snake( common::ID id, common::Coordinate x, common::Coordinate y ) :
    m_id{ id }{
    m_currentDirectionSnake = common::DirectionSnake::MOTIONLESS;
    m_tail.push_back( common::Point{ x, y } );
    m_tail.push_back( common::Point{ x, y } );
}

std::size_t flower::Snake::getSizeTail() const {
    return m_tail.size();
}

flower::common::Coordinate flower::Snake::getCoordinateHeadX() const {
    return m_tail[0].x;
}

flower::common::Coordinate flower::Snake::getCoordinateHeadY() const {
    return m_tail[0].y;
}

flower::common::DirectionSnake flower::Snake::getCurrentDirectionSnake() const {
    return m_currentDirectionSnake;
}

std::vector< flower::common::Point > flower::Snake::getCoordinateTail() const {
    return m_tail;
}

flower::common::ID flower::Snake::getId() const {
    return m_id;
}

void flower::Snake::setCurrentDirectionSnake( const common::DirectionSnake currentDirectionSnake ){
    m_currentDirectionSnake = currentDirectionSnake;
}

void flower::Snake::move(){
    switch ( m_currentDirectionSnake )
    {
        case common::DirectionSnake::LEFT: movingItem( -1, 0 ); break;
        case common::DirectionSnake::RIGHT: movingItem( 1, 0 ); break;
        case common::DirectionSnake::UP: movingItem( 0, 1 ); break;
        case common::DirectionSnake::DOWN: movingItem( 0, -1 ); break;
        default: break;
    }
}

void flower::Snake::incrementTail(){
    m_tail.push_back( common::Point{ m_tail.begin()->x, m_tail.begin()->y } );
}

void flower::Snake::decrementTail(){
    m_tail.pop_back();
}

void flower::Snake::movingItem( const common::Coordinate x, const common::Coordinate y ){
    for ( std::size_t i = m_tail.size();; --i ) {

        if ( i == 1 ) {
            m_tail[ 0 ].x += x;
            m_tail[ 0 ].y -= y;
            return;
        }
        int flag = true;
        if ( i == m_tail.size() ) {
            for (int j = 0; j < m_tail.size() - 1; ++j) {
                if ( m_tail[ m_tail.size() - 1 ].x == m_tail[ j ].x &&
                    m_tail[ m_tail.size() - 1 ].y == m_tail[ j ].y ) {
                    flag = false;
                }
            }
            if ( flag ) {
                m_tail[ i - 1 ].x = m_tail[ i - 2 ].x;
                m_tail[ i - 1 ].y = m_tail[ i - 2 ].y;
            }
        }
        else {
            m_tail[ i - 1 ].x = m_tail[ i - 2 ].x;
            m_tail[ i - 1 ].y = m_tail[ i - 2 ].y;
        }
    }
}
