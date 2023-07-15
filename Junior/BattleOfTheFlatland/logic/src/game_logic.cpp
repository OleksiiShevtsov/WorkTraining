#include "logic/game_logic.h"

// TODO delete
#include <iostream>

bof::GameLogic::GameLogic() :
    m_gameState{ common::GameState::START }
{
    m_dynamicObject.push_back( DynamicObject( 0, common::Point{ 100, 300 } ) );
}

void bof::GameLogic::startGame()
{
    while (m_appRendering.isWindowOpen())
    {
        gameState();
        m_appRendering.draw();
    }
    closeGame();
}

void bof::GameLogic::join()
{
    m_appRendering.getInputHandler()->join();
}

void bof::GameLogic::closeGame()
{
    m_gameState = common::GameState::END;
    m_appRendering.getInputHandler()->setGameState( m_gameState );
}

// TODO client-server
void bof::GameLogic::getPlayerMovementStateFromApp()
{
    for ( auto& obj : m_dynamicObject )
    {
        if ( obj.getId() == m_appRendering.getPlayer()->getId())
        {
            obj.setMovementState( m_appRendering.getInputHandler()->getMovementState() );
        }
    }
}

void bof::GameLogic::sendStateDynamicObjectToAppRender()
{
    for( const auto &obj : m_dynamicObject )
    {
        m_appRendering.setDynamicObject( obj.getId(), obj.getDot(), obj.getAngle() );
    }
}

void bof::GameLogic::gameState()
{
    getPlayerMovementStateFromApp();

    for ( auto& obj : m_dynamicObject ) 
    {
        obj.recalculationDot();
    }

    sendStateDynamicObjectToAppRender();
}
