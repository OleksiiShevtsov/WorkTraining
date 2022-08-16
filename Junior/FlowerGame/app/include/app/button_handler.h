#pragma once

#include <SFML/Graphics.hpp>
#include <thread>
#include <memory>
#include "windows.h"

#include "common/enum.h"
#include "common/global_settings.h"
#include "common/function.h"

namespace flower::app {

    class ButtonHandler {

    public:
        ButtonHandler( std::shared_ptr< sf::RenderWindow > window );

        common::Point getPressurePoint() const;
        common::ButtonState getButtonState() const;

        void setButtonState( common::ButtonState buttonState );
        void setGameState( common::GameState gameState );

        void join();

   private:

        void handlerClick( std::shared_ptr< sf::RenderWindow > window );

        common::GameState m_gameState;
        std::unique_ptr< std::thread > m_handlerClick;
        common::ButtonState m_buttonState;
        common::Point m_pressurePoint;
    };
}
