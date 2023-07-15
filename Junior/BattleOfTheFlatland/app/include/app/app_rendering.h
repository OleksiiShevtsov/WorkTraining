#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "app/input_handler.h"
#include "common/global_settings.h"
#include "player.h"
#include "types.h"

namespace bof::app 
{
    class AppRendering 
    {
    public:

        AppRendering( const AppRendering& ) = delete;
        AppRendering( AppRendering&& ) = delete;

        AppRendering();

        std::shared_ptr< Player > getPlayer() const;
        std::shared_ptr< InputHandler > getInputHandler() const;

        void setDynamicObject( const common::ID id, common::Point dot, common::Angle alpha );

        bool isWindowOpen() const;
        void draw();

    private:

        void drowMap();
        void drawDynamicObject();

        std::shared_ptr< Player > m_player;
        std::vector< DynamicObject > m_dynamicObject;
        std::shared_ptr< InputHandler > m_inputHandler;
        std::shared_ptr< sf::RenderWindow > m_window;
    };
}
