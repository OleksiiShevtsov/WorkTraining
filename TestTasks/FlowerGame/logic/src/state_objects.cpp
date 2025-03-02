#include "logic/state_objects.h"

flower::StateObjects::StateObjects( const uint32_t pixelCountX, const uint32_t pixelCountY ) :
    m_pixelCountX{ pixelCountX },
    m_pixelCountY{ pixelCountY },
    m_idObject{ 0 },
    m_snakeSpeed{ common::SnakeSpeed::NORMAL }{}

flower::common::ID flower::StateObjects::getIdObject(){
    return m_idObject;
}

std::vector< flower::Snake > flower::StateObjects::getSnakes() const {
    return m_snakes;
}

std::vector< flower::Flower > flower::StateObjects::getFlowers() const {
    return m_flowers;
}

flower::common::SnakeSpeed flower::StateObjects::getSnakeSpeed() const {
    return m_snakeSpeed;
}

flower::common::ID flower::StateObjects::getIdBySnakeCoordinates( const common::Coordinate x, const common::Coordinate y ){
    for( auto &snake : m_snakes ){
        for( auto &tail : snake.getCoordinateTail() ){
            if( tail.x == x && tail.y == y ){
                return snake.getId();
            }
        }
    }

    // TODO handle error
    return 0;
}

void flower::StateObjects::setSnakeSpeed( const common::SnakeSpeed snakeSpeed ){
    m_snakeSpeed = snakeSpeed;
}

std::chrono::milliseconds flower::StateObjects::currentSnakeSpeed(){
    switch( m_snakeSpeed ){
        case common::SnakeSpeed::NORMAL: return common::GlobalSettings::snakeSpeedNormal; break;
        case common::SnakeSpeed::FORCE: return common::GlobalSettings::snakeSpeedForce; break;
        default: return {};
    };
}

void flower::StateObjects::emergenceOfNewSnake(){
    while( true ){
        srand(time(NULL));
        common::Coordinate x = rand() % common::GlobalSettings::pixelCountX;
        common::Coordinate y = rand() % common::GlobalSettings::pixelCountY;
        if( !snakeByCoordinates( x, y ) && !flowerByCoordinates( x, y ) ){
            m_snakes.push_back( Snake( ++m_idObject, x, y ) );
            break;
        }
    }
}

bool flower::StateObjects::snakeByCoordinates( const common::Coordinate x, const common::Coordinate y ){
    for( auto &snake : m_snakes ){
        for( auto &tail : snake.getCoordinateTail() ){
            if( tail.x == x && tail.y == y ){
                return true;
            }
        }
    }
    return false;
}

void flower::StateObjects::trimmingSnakeById( const common::ID id ){
    if( id == 0 ){
        return;
    }
    auto end = m_snakes.end();
    for( auto snake = m_snakes.begin(); snake != end; snake++ ){
        if( snake->getId() == id ){
            snake->decrementTail();
            if( snake->getCoordinateTail().size() == 0 ){
                m_snakes.erase( snake );
            }
            return;
        }
    }
}

void flower::StateObjects::eraseSnakeById( const common::ID id ){
    if( id == 0 ){
        return;
    }
    auto end = m_snakes.end();
    for( auto snake = m_snakes.begin(); snake != end; snake++ ){
        if( snake->getId() == id ){
            m_snakes.erase( snake );
            return;
        }
    }
}

void flower::StateObjects::emergenceOfNewFlower( const common::Coordinate x, const common::Coordinate y ){
    m_flowers.push_back( Flower( ++m_idObject, x, y ) );
}

bool flower::StateObjects::flowerByCoordinates( const common::Coordinate x, const common::Coordinate y ){
    for( auto &flower : m_flowers ){
        if( flower.getCoordinateFlower().x == x && flower.getCoordinateFlower().y == y ){
            return true;
        }
    }
    return false;
}

void flower::StateObjects::removeFlowerByCoordinates( const common::Coordinate x, const common::Coordinate y  ){
    auto end = m_flowers.end();
    for( auto flower = m_flowers.begin(); flower != end; flower++ ){
        if( flower->getCoordinateFlower().x == x && flower->getCoordinateFlower().y == y ){
            m_flowers.erase( flower );
            return;
        }
    }
}

void flower::StateObjects::snakesGrowth(){
    for( auto &snake : m_snakes ){
        if( m_flowers.size() > 1 ){
            if( snake.getSizeTail() < 10 ){
                snake.incrementTail();
            }
        }
        else{
            if( snake.getSizeTail() < 5 ){
                snake.incrementTail();
            }
        }
    }
}

bool flower::StateObjects::directionCheckForSnake( const common::DirectionSnake directionToCheck, const Snake snake ){
    switch( directionToCheck ){
        case common::DirectionSnake::LEFT: if( snakeByCoordinates( snake.getCoordinateHeadX() - 1, snake.getCoordinateHeadY() ) ){ return true; }; break;
        case common::DirectionSnake::RIGHT: if( snakeByCoordinates( snake.getCoordinateHeadX() + 1, snake.getCoordinateHeadY() ) ){ return true; }; break;
        case common::DirectionSnake::UP: if( snakeByCoordinates( snake.getCoordinateHeadX(), snake.getCoordinateHeadY() - 1 ) ){ return true; }; break;
        case common::DirectionSnake::DOWN: if( snakeByCoordinates( snake.getCoordinateHeadX(), snake.getCoordinateHeadY() + 1 ) ){ return true; }; break;
        default : break;
    }
    return false;
}

void flower::StateObjects::movingSnakeToNextCoordinate(){
    for( auto &snake : m_snakes ){

        // the snake does not leave the map, does not cross other snakes including itself and cannot move in the opposite direction
        std::vector<common::DirectionSnake> availableDirection;
        if( snake.getCurrentDirectionSnake() != common::DirectionSnake::LEFT && snake.getCoordinateHeadX() + 1 < m_pixelCountX && !directionCheckForSnake( common::DirectionSnake::RIGHT, snake ) ){
            availableDirection.push_back( common::DirectionSnake::RIGHT );
        }
        if( snake.getCurrentDirectionSnake() != common::DirectionSnake::RIGHT && snake.getCoordinateHeadX() > 0 && !directionCheckForSnake( common::DirectionSnake::LEFT, snake ) ){
            availableDirection.push_back( common::DirectionSnake::LEFT );
        }
        if( snake.getCurrentDirectionSnake() != common::DirectionSnake::UP && snake.getCoordinateHeadY() + 1 < m_pixelCountY && !directionCheckForSnake( common::DirectionSnake::DOWN, snake ) ){
            availableDirection.push_back( common::DirectionSnake::DOWN );
        }
        if( snake.getCurrentDirectionSnake() != common::DirectionSnake::DOWN && snake.getCoordinateHeadY() > 0 && !directionCheckForSnake( common::DirectionSnake::UP, snake ) ){
            availableDirection.push_back( common::DirectionSnake::UP );
        }

        // if there is no free direction, then the snake disappears
        if( availableDirection.size() == 0 ){
            eraseSnakeById( snake.getId() );
            return;
        }

        snake.setCurrentDirectionSnake( availableDirection[ rand() % availableDirection.size() ] );
        snake.move();

        if( flowerByCoordinates( snake.getCoordinateHeadX(), snake.getCoordinateHeadY() ) ){
            removeFlowerByCoordinates( snake.getCoordinateHeadX(), snake.getCoordinateHeadY() );
            m_snakeSpeed = common::SnakeSpeed::FORCE;
        }
    }
}
