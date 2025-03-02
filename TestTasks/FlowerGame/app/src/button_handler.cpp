#include "app/button_handler.h"

flower::app::ButtonHandler::ButtonHandler( std::shared_ptr< sf::RenderWindow > window ) :
    m_gameState{ common::GameState::START },
    m_buttonState{ common::ButtonState::FREE }{
    m_handlerClick = std::make_unique< std::thread >( &ButtonHandler::handlerClick, this, window );
}

flower::common::Point flower::app::ButtonHandler::getPressurePoint() const {
    return m_pressurePoint;
}

flower::common::ButtonState flower::app::ButtonHandler::getButtonState() const {
    return m_buttonState;
}

void flower::app::ButtonHandler::setButtonState( common::ButtonState buttonState ){
    m_buttonState = buttonState;
}

void flower::app::ButtonHandler::setGameState( common::GameState gameState ){
    m_gameState = gameState;
}

void flower::app::ButtonHandler::join(){
    m_handlerClick->join();
}

void flower::app::ButtonHandler::handlerClick( std::shared_ptr< sf::RenderWindow > window ){
    while( m_gameState != common::GameState::END ){
        if ( sf::Mouse::isButtonPressed( sf::Mouse::Right ) && m_buttonState == common::ButtonState::FREE )
        {
            m_pressurePoint.x = sf::Mouse::getPosition( *window.get() ).x / common::GlobalSettings::sizePixel;
            m_pressurePoint.y = sf::Mouse::getPosition( *window.get() ).y / common::GlobalSettings::sizePixel;
            if( m_pressurePoint.x < common::GlobalSettings::pixelCountX && m_pressurePoint.y < common::GlobalSettings::pixelCountY ){
                m_buttonState = common::ButtonState::PRESSED;
            }
            common::waiting( common::GlobalSettings::pauseBetweenButtonPresses );
        }
    }
}
