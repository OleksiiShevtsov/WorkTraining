#pragma once

#include <logic/state_objects.h>
#include <app/app_rendering.h>

#include "logic/dynamic_object.h"

namespace bof
{
    class GameLogic {

    public:
        GameLogic( const GameLogic& ) = delete;
        GameLogic( GameLogic&& ) = delete;

        GameLogic();

        void startGame();

        void join();

    private:

        void closeGame();

        void getPlayerMovementStateFromApp();
        void sendStateDynamicObjectToAppRender();
        void gameState();

        std::vector< DynamicObject > m_dynamicObject;
        app::AppRendering m_appRendering;
        common::GameState m_gameState;
    };
}
