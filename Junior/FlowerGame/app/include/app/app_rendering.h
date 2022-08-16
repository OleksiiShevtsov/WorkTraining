#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

// TODO delete
#include <iostream>

#include "app/button_handler.h"
#include "common/global_settings.h"
#include "app_settings.h"
#include "types.h"

namespace flower::app {

    class AppRendering {

    public:

        AppRendering( const AppRendering& ) = delete;
        AppRendering( const AppRendering&& ) = delete;

        AppRendering();

        std::shared_ptr< ButtonHandler > getButtonHandler() const;

        void setSnakeObjects( const common::ID id, const std::vector< common::Point > snake );
        void setFlowerObjects( const common::ID id, const common::Point flower );
        void setTextNumberOfCoins( const common::Coin coin );
        void setTextInStatisticsBox( const std::string text );

        bool isWindowOpen() const;
        void drawObjects();

    private:

        void drowSnake();
        void drowFlower();
        void drowMap();

        std::string m_textNumberOfCoins;
        std::string m_textInStatisticsBox;
        std::vector< ObjSnake > m_snakeObjects;
        std::vector< ObjFlower >m_flowerObjects;
        std::shared_ptr< ButtonHandler > m_buttonHandler;
        std::shared_ptr< sf::RenderWindow > m_window;
    };
}
