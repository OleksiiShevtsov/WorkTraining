#pragma once

#include <SFML/Graphics.hpp>
#include <thread>
#include <memory>
#include <functional>
// #include "windows.h"

#include "common/enum.h"
#include "common/global_settings.h"
#include "common/function.h"

namespace bof::app 
{
    class InputHandler
    {
    public:
        InputHandler( std::shared_ptr< sf::RenderWindow > window );

        common::MovementState getMovementState() const;

        void setGameState( common::GameState gameState );

        void join();

   private:

        void handlerKeyboard( std::shared_ptr< sf::RenderWindow > window );

        common::MovementState m_movementState;
        common::GameState m_gameState;
        std::unique_ptr< std::thread > m_handlerClick;
    };
}
