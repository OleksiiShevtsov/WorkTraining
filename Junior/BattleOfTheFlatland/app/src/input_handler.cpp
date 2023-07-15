#include "app/input_handler.h"

bof::app::InputHandler::InputHandler( std::shared_ptr< sf::RenderWindow > window )
{
    m_handlerClick = std::make_unique< std::thread >( &InputHandler::handlerKeyboard, this, window );
}

bof::common::MovementState bof::app::InputHandler::getMovementState() const
{
    return m_movementState;
}

void bof::app::InputHandler::setGameState( common::GameState gameState ) {
    m_gameState = gameState;
}

void bof::app::InputHandler::join(){
    m_handlerClick->join();
}

void bof::app::InputHandler::handlerKeyboard( std::shared_ptr< sf::RenderWindow > window ){
    while ( m_gameState != common::GameState::END ) {
        if( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) )
        {
            m_movementState = common::MovementState::UP;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            m_movementState = common::MovementState::DOWN;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            m_movementState = common::MovementState::RIGHT;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            m_movementState = common::MovementState::LEFT;
        }
        else {
            m_movementState = common::MovementState::FREE;
        }
    }
}
