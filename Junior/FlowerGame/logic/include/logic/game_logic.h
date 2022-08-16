#pragma once

#include <logic/state_objects.h>
#include <app/app_rendering.h>

namespace flower {

    class GameLogic {

    public:
        GameLogic( const GameLogic& ) = delete;
        GameLogic( const GameLogic&& ) = delete;

        GameLogic();

        void startGame();
        void join();

    private:

        void closeGame();

        // state control threads
        void coinCounting();
        void snakeState();
        void snakeGrowth();
        void newSnake();
        void snakeSpeed();
        void reductionAppearanceNewSnake();

        void passObjectsToAppRender();
        void gameState();

        common::GameState m_gameState;
        common::Coin m_coin;
        std::chrono::milliseconds m_timeForNewSnake;
        common::SnakeState m_snakeState;
        common::SnakeGrowth m_snakeGrowth;
        common::NewSnake m_newSnake;
        std::unique_ptr< std::thread > m_coinCountingFunction;
        std::unique_ptr< std::thread > m_snakeStateFunction;
        std::unique_ptr< std::thread > m_snakeGrowthFunction;
        std::unique_ptr< std::thread > m_newSnakeFunction;
        std::unique_ptr< std::thread > m_snakeSpeedFunction;
        std::unique_ptr< std::thread > m_reductionAppearanceNewSnakeFunction;
        StateObjects m_stateObjects{ common::GlobalSettings::pixelCountX, common::GlobalSettings::pixelCountY };
        app::AppRendering m_appRendering;
    };
}
