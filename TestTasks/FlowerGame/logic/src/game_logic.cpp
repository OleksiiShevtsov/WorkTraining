#include "logic/game_logic.h"

flower::GameLogic::GameLogic() :
    m_gameState{ common::GameState::START },
    m_coin{ common::GlobalSettings::Coins::initialSupplyOfCoins },
    m_timeForNewSnake{ common::GlobalSettings::timeForNewSnake },
    m_snakeState{ common::SnakeState::STAND },
    m_snakeGrowth{ common::SnakeGrowth::STAND },
    m_newSnake{ common::NewSnake::STAND }{
    m_snakeStateFunction = std::make_unique< std::thread >( &GameLogic::snakeState, this );
    m_snakeGrowthFunction = std::make_unique< std::thread >( &GameLogic::snakeGrowth, this );
    m_newSnakeFunction = std::make_unique< std::thread >( &GameLogic::newSnake, this );
    m_snakeSpeedFunction = std::make_unique< std::thread >( &GameLogic::snakeSpeed, this );
}

void flower::GameLogic::startGame(){
    while (m_appRendering.isWindowOpen())
    {
        gameState();
        m_appRendering.drawObjects();
    }
    closeGame();
}

void flower::GameLogic::join(){
    m_coinCountingFunction->join();
    m_snakeStateFunction->join();
    m_snakeGrowthFunction->join();
    m_newSnakeFunction->join();
    m_snakeSpeedFunction->join();
    m_reductionAppearanceNewSnakeFunction->join();
    m_appRendering.getButtonHandler()->join();
}

void flower::GameLogic::closeGame(){
    m_gameState = common::GameState::END;
    m_appRendering.getButtonHandler()->setGameState( m_gameState );
    m_appRendering.setTextInStatisticsBox( "game over" );
}

void flower::GameLogic::coinCounting(){
    while(  m_gameState != common::GameState::END  ){
        common::waiting( common::GlobalSettings::timeForNewCoins );
        m_coin = m_coin + common::GlobalSettings::Coins::coinsForOneFlower * m_stateObjects.getFlowers().size();
        m_appRendering.setTextNumberOfCoins( m_coin );
    }
}

void flower::GameLogic::snakeState(){
    while(  m_gameState != common::GameState::END  ){
        if( m_snakeState == common::SnakeState::READY ){
            common::waiting( m_stateObjects.currentSnakeSpeed() );
            m_snakeState = common::SnakeState::READY;
        }
    }
}

void flower::GameLogic::snakeGrowth(){
    while(  m_gameState != common::GameState::END  ){
        if( m_snakeGrowth == common::SnakeGrowth::READY ){
            common::waiting( common::GlobalSettings::timeForSnakeGrowth );
            m_snakeGrowth = common::SnakeGrowth::READY;
        }
    }
}

void flower::GameLogic::newSnake(){
    while(  m_gameState != common::GameState::END  ){
        if( m_newSnake == common::NewSnake::READY ){
            common::waiting( m_timeForNewSnake );
            m_newSnake = common::NewSnake::READY;
        }
    }
}

void flower::GameLogic::snakeSpeed(){
    while(  m_gameState != common::GameState::END  ){
        if( m_stateObjects.getSnakeSpeed() == common::SnakeSpeed::FORCE ){
            common::waiting( common::GlobalSettings::timeForSpeedForce );
            m_stateObjects.setSnakeSpeed( common::SnakeSpeed::NORMAL );
        }
    }
}

void flower::GameLogic::reductionAppearanceNewSnake(){
    while(  m_gameState != common::GameState::END  ){
        common::waiting( common::GlobalSettings::timeToDecreaseTimeOfAppearanceNewSnake );
        m_timeForNewSnake = std::chrono::milliseconds{ static_cast< int >( m_timeForNewSnake.count() * common::GlobalSettings::coefficientOfReductionAppearanceNewSnake ) };
    }
}

void flower::GameLogic::passObjectsToAppRender(){
    for( const auto &snake : m_stateObjects.getSnakes() ){
        m_appRendering.setSnakeObjects( snake.getId(), snake.getCoordinateTail() );
    }
    for( const auto &flower : m_stateObjects.getFlowers() ){
        m_appRendering.setFlowerObjects( flower.getId(), flower.getCoordinateFlower() );
    }
}

void flower::GameLogic::gameState(){

    if( m_appRendering.getButtonHandler()->getButtonState() == common::ButtonState::PRESSED ){

        if( m_stateObjects.snakeByCoordinates( m_appRendering.getButtonHandler()->getPressurePoint().x, m_appRendering.getButtonHandler()->getPressurePoint().y ) ){
            m_stateObjects.trimmingSnakeById( m_stateObjects.getIdBySnakeCoordinates( m_appRendering.getButtonHandler()->getPressurePoint().x, m_appRendering.getButtonHandler()->getPressurePoint().y ) );
        }

        else if( !m_stateObjects.flowerByCoordinates( m_appRendering.getButtonHandler()->getPressurePoint().x, m_appRendering.getButtonHandler()->getPressurePoint().y ) &&
                 m_coin >= common::GlobalSettings::Coins::flowerCost ){
            m_stateObjects.emergenceOfNewFlower( m_appRendering.getButtonHandler()->getPressurePoint().x, m_appRendering.getButtonHandler()->getPressurePoint().y );
            m_coin = m_coin - common::GlobalSettings::Coins::flowerCost;
            m_appRendering.setTextNumberOfCoins( m_coin );
            if( m_gameState == common::GameState::START ){

                // start game
                m_gameState = common::GameState::PROCESS;
                m_snakeGrowth = common::SnakeGrowth::READY;
                m_snakeState = common::SnakeState::READY;
                m_newSnake = common::NewSnake::READY;
                m_appRendering.setTextNumberOfCoins( m_coin );
                m_appRendering.setTextInStatisticsBox( "cost a flower is 50 coins" );
                m_reductionAppearanceNewSnakeFunction = std::make_unique< std::thread >( &GameLogic::reductionAppearanceNewSnake, this );
                m_coinCountingFunction = std::make_unique< std::thread >( &GameLogic::coinCounting, this );
            }
        }

        m_appRendering.getButtonHandler()->setButtonState( common::ButtonState::FREE );
    }

    if( m_snakeState == common::SnakeState::READY && m_gameState != common::GameState::END ){
        m_stateObjects.movingSnakeToNextCoordinate();
        m_snakeState = common::SnakeState::STAND;
    }

    if( m_stateObjects.getIdObject() != 0 && m_stateObjects.getFlowers().size() == 0 ){
        closeGame();
    }

    if( m_snakeGrowth == common::SnakeGrowth::READY && m_gameState != common::GameState::END ){
        m_stateObjects.snakesGrowth();
        m_snakeGrowth = common::SnakeGrowth::STAND;
    }

    if( m_newSnake == common::NewSnake::READY && m_gameState != common::GameState::END ){
        m_stateObjects.emergenceOfNewSnake();
        m_newSnake = common::NewSnake::STAND;
    }

    passObjectsToAppRender();
}
