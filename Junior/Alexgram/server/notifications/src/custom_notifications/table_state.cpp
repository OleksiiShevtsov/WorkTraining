#include "notifications/custom_notifications/table_state.h"
#include "logic/enums/to_string.h"

#include <g3log/g3log.hpp>

#include <algorithm>
#include <chrono>
#include <map>

using namespace sylk;
using namespace Notification;
using namespace sylk::common;

ActionsOptions::ActionsOptions( sylk::ActionOptions actionOptions )
    : m_minBet( actionOptions.getMinBet().getMoney() )
    , m_maxBet( actionOptions.getMaxBet().getMoney() )
{
    std::for_each( actionOptions.getAvailableActions().begin(),
                   actionOptions.getAvailableActions().end(),
                   [ this ]( const auto& action ) {
                       m_seatActions.push_back( action );
                   } );
}

int64_t ActionsOptions::getMinBet() const
{
    return m_minBet;
}

int64_t ActionsOptions::getMaxBet() const
{
    return m_maxBet;
}

std::vector< SeatAction > ActionsOptions::getSeatsActions() const
{
    return m_seatActions;
}
PreActionsOptions::PreActionsOptions( sylk::PreActionOptions preActionOptions )
{
    std::for_each( preActionOptions.getAvailablePreActions().begin(),
                   preActionOptions.getAvailablePreActions().end(),
                   [ this ]( const auto& preAction ) {
                       m_seatPreActions.push_back( preAction );
                   } );
}
std::vector< sylk::PreAction > PreActionsOptions::getSeatsPreActions() const
{
    return m_seatPreActions;
}

PlayerOnShowdownState::PlayerOnShowdownState( IDX seatIdx,
                                              bool muck,
                                              const std::vector< std::string >& cards )
    : m_seatIdx( seatIdx )
    , m_muck( muck )
    , m_cards( cards )
{
}

IDX PlayerOnShowdownState::getSeatIdx() const
{
    return m_seatIdx;
}

bool PlayerOnShowdownState::getMuck() const
{
    return m_muck;
}

std::vector< std::string > PlayerOnShowdownState::getCards() const
{
    return m_cards;
}

void PlayerOnShowdownState::setMuck( bool muck )
{
    m_muck = muck;
}

void PlayerOnShowdownState::setCards( const std::vector< std::string >& cards )
{
    m_cards = cards;
}

PlayerInAllInState::PlayerInAllInState( ID seatIdx, const std::vector< std::string >& cards )
    : m_seatIdx( seatIdx )
    , m_cards( cards )
{
}

IDX PlayerInAllInState::getSeatIdx() const
{
    return m_seatIdx;
}

std::vector< std::string > PlayerInAllInState::getCards() const
{
    return m_cards;
}

void PlayerInAllInState::setCards( const std::vector< std::string >& cards )
{
    m_cards = cards;
}

double PlayerInAllInState::getWinPercentage() const
{
    return m_winPercentage;
}

void PlayerInAllInState::setWinPercentage( double winPercentage )
{
    m_winPercentage = winPercentage;
}

WinnerState::WinnerState( IDX seatIdx, uint64_t moneyReward )
    : m_seatIdx( seatIdx )
    , m_moneyReward( moneyReward )
{
}

WinnerState::WinnerState( IDX seatIdx,
                          uint64_t moneyReward,
                          const std::vector< SidePotReward >& sidePotShortStackPerAmount )
    : m_seatIdx( seatIdx )
    , m_moneyReward( moneyReward )
{
    for ( const auto& sidePotRewardItem : sidePotShortStackPerAmount )
    {
        m_sidePotShortStackPerAmount.emplace_back( sidePotRewardItem.m_amount,
                                                   sidePotRewardItem.m_shortStack );
    }
}

IDX WinnerState::getSeatIdx() const
{
    return m_seatIdx;
}

uint64_t WinnerState::getMoneyReward() const
{
    return m_moneyReward;
}

std::vector< SidePotShortStackPerAmount > WinnerState::getSidePotShortStackPerAmount() const
{
    return m_sidePotShortStackPerAmount;
}

std::optional< Notification::ActionsOptions > Seat::getActionsOptions() const
{
    return m_actionsOptions;
}

std::optional< Notification::PreActionsOptions > Seat::getPreActionsOptions() const
{
    return m_preActionsOptions;
}

HandPlayedInfo::HandPlayedInfo( const std::string& fileName,
                                const std::string& handMongoId,
                                const std::string& handHeader,
                                const std::string& handBody )
    : m_fileName( fileName )
    , m_handMongoId( handMongoId )
    , m_handHeader( handHeader )
    , m_handBody( handBody )
{
}

const std::string& HandPlayedInfo::getHandMongoId() const
{
    return m_handMongoId;
}

const std::string& HandPlayedInfo::getHandHeader() const
{
    return m_handHeader;
}

const std::string& HandPlayedInfo::getHandBody() const
{
    return m_handBody;
}

const std::string& HandPlayedInfo::getFileName() const
{
    return m_fileName;
}

ShowdownState::ShowdownState( const std::vector< WinnerState >& winners,
                              const std::vector< PlayerOnShowdownState >& playerShowdown,
                              HandPlayedInfo handPlayedInfo )
    : m_winners( winners )
    , m_playerOnShowdown( playerShowdown )
    , m_handPlayedInfo( handPlayedInfo )
{
}

std::vector< WinnerState > ShowdownState::getWinners() const
{
    return m_winners;
}

std::vector< PlayerOnShowdownState > ShowdownState::getShowdownPlayers() const
{
    return m_playerOnShowdown;
}

HandPlayedInfo ShowdownState::getHandPlayedInfo() const
{
    return m_handPlayedInfo;
}

void ShowdownState::reset()
{
    m_winners.clear();
    m_playerOnShowdown.clear();
}

CardsShowState::CardsShowState( const std::vector< PlayerInAllInState >& playerInAllIn )
    : m_playerInAllIn( playerInAllIn )
{
}

std::vector< PlayerInAllInState > CardsShowState::getPlayersInAllIn() const
{
    return m_playerInAllIn;
}

void CardsShowState::reset()
{
    m_playerInAllIn.clear();
}

TableState::TableState( ID tableId ) : m_tableId( tableId ) { }

TableState::TableState( ID tableId,
                        const std::string& tableName,
                        IDX seatIdxBB,
                        IDX seatIdxSB,
                        uint64_t bbAmount,
                        uint64_t sbAmount,
                        GameState gameState,
                        const std::vector< std::string >& communityCards,
                        const std::vector< Seat >& seats,
                        ShowdownState showdownState )

    : m_tableId( tableId )
    , m_tableName( tableName )
    , m_seatIdxBB( seatIdxBB )
    , m_seatIdxSB( seatIdxSB )
    , m_bbAmount( bbAmount )
    , m_sbAmount( sbAmount )
    , m_gameState( gameState )
    , m_communityCards( communityCards )
    , m_seats( seats )
    , m_showdownState( showdownState )
{
}

std::optional< ID > TableState::getTableId() const
{
    return m_tableId;
}

std::string TableState::getTableName() const
{
    return m_tableName;
}

IDX TableState::getSeatIdxBb() const
{
    return m_seatIdxBB;
}

IDX TableState::getSeatIdxSb() const
{
    return m_seatIdxSB;
}

IDX TableState::getSeatIdxButton() const
{
    return m_seatIdxButton;
}

Pot TableState::getPot() const
{
    return m_pot;
}

uint64_t TableState::getBbAmount() const
{
    return m_bbAmount;
}

uint64_t TableState::getSbAmount() const
{
    return m_sbAmount;
}

Notification::GameState TableState::getGameState() const
{
    return m_gameState;
}

std::vector< Notification::Seat >& TableState::getSeats()
{
    return m_seats;
}

std::vector< std::string >& TableState::getCommunityCards()
{
    return m_communityCards;
}

void TableState::setPlayerId( ID playerId )
{
    m_playerId = playerId;
}

void TableState::setTableName( const std::string& tableName )
{
    m_tableName = tableName;
}

void TableState::setSeatIdxBb( IDX seatIdxBb )
{
    m_seatIdxBB = seatIdxBb;
}

void TableState::setSeatIdxSb( IDX seatIdxSb )
{
    m_seatIdxSB = seatIdxSb;
}

void TableState::setSeatIdxButton( IDX seatIdxButton )
{
    m_seatIdxButton = seatIdxButton;
}

void TableState::setPot( const Pot& pot )
{
    m_pot = pot;
}

void TableState::setBbAmount( Money bbAmount )
{
    m_bbAmount = bbAmount.getMoney();
}

void TableState::setGameState( GameState gameState )
{
    m_gameState = gameState;
}

void TableState::reset()
{
    m_seatIdxBB     = 0;
    m_seatIdxSB     = 0;
    m_seatIdxButton = 0;

    m_gameState = GameState::Default;
    m_playersOnShowdown.clear();
    m_showdownState.reset();
    m_pot.reset();
    m_communityCards.clear();
    m_playersInAllIn.clear();
    m_cardsShowState.reset();
}

ID TableState::getPlayerId() const
{
    return m_playerId;
}

void TableState::setShowdownState( ShowdownState showdownState )
{
    m_showdownState = showdownState;
}

ShowdownState TableState::getShowdownState() const
{
    return m_showdownState;
}

CardsShowState TableState::getCardsShowState() const
{
    return m_cardsShowState;
}

void TableState::setCardsShowState( CardsShowState cardsShowState )
{
    m_cardsShowState = cardsShowState;
}

void TableState::setSeatIsDisconnected( common::IDX seatIdx, bool value )
{
    findSeat( seatIdx ).setIsDisconnected( value );
}

void TableState::clearSeatState( common::IDX seatIdx )
{
    findSeat( seatIdx ).clearDisconnect();
}

void TableState::setSeatIsFolded( IDX seatIdx, bool value )
{
    findSeat( seatIdx ).setIsFolded( value );
}

void TableState::setSeatIsPlaying( IDX seatIdx, bool value )
{
    findSeat( seatIdx ).setIsPlaying( value );
}

void TableState::setSeatMuckAvailability( IDX seatIdx, bool value )
{
    findSeat( seatIdx ).setMuckAvailability( value );
}

void TableState::setCanRebuy( common::IDX seatIdx, bool value )
{
    findSeat( seatIdx ).setCanRebuy( value );
}

void TableState::setSeatInSitout( IDX seatIdx, bool value )
{
    findSeat( seatIdx ).setInSitout( value );
}

void TableState::setSeatSeatIdx( IDX seatIdx )
{
    findSeat( seatIdx ).setSeatIdx( seatIdx );
}

void TableState::setSeatName( IDX seatIdx, std::string name )
{
    findSeat( seatIdx ).setName( name );
}

void TableState::setSeatAvatar( IDX seatIdx, const std::string& avatar )
{
    findSeat( seatIdx ).setUserAvatar( avatar );
}

void TableState::addSeatBet( IDX seatIdx, uint64_t bet )
{
    findSeat( seatIdx ).addBet( bet );
}

void TableState::setSeatCards( IDX seatIdx, std::vector< Card > cards )
{
    findSeat( seatIdx ).setCards( cards );
}

void TableState::seatClearCards( IDX seatIdx )
{
    findSeat( seatIdx ).clearCards();
}

void TableState::setSeatStake( IDX seatIdx, uint64_t stake )
{
    findSeat( seatIdx ).setStake( stake );
}

void TableState::setSeatActionOptions( IDX seatIdx, const sylk::ActionOptions& actionOptions )
{
    findSeat( seatIdx ).setActionsOptions( actionOptions );
}

void TableState::setSeatPreActionOptions( IDX seatIdx,
                                          const sylk::PreActionOptions& preActionOptions )
{
    findSeat( seatIdx ).setPreActionsOptions( preActionOptions );
}

void TableState::setSeatLastAction( IDX seatIdx, SeatAction action )
{
    findSeat( seatIdx ).setLastAction( sylk::to_string( action ) );
}

void TableState::setSeatLastPreAction( IDX seatIdx, PreAction preAction )
{
    findSeat( seatIdx ).setLastPreAction( sylk::to_string( preAction ) );
}

void TableState::seatClearLastAction( IDX seatIdx )
{
    findSeat( seatIdx ).clearLastAction();
}

void TableState::seatClearLastPreAction( IDX seatIdx )
{
    findSeat( seatIdx ).clearLastPreAction();
}

void TableState::clearAllSeatsLastAction()
{
    std::for_each( m_seats.begin(), m_seats.end(), []( auto& seat ) {
        seat.clearLastAction();
    } );
}

void TableState::clearAllSeatsLastPreAction()
{
    std::for_each( m_seats.begin(), m_seats.end(), []( auto& seat ) {
        seat.clearLastPreAction();
    } );
}

void TableState::setSeatActionTimeLimit( std::chrono::milliseconds actionTimeLimit )
{
    m_actionTimeLimit = actionTimeLimit.count();
}

void TableState::setTimebankActivated( bool timebankActivated )
{
    m_timebankActivated = timebankActivated;
}

void TableState::setPlayersOnShowdown(
    const std::vector< Notification::PlayerOnShowdownState >& playersOnShowdown )
{
    m_playersOnShowdown = playersOnShowdown;
}

void TableState::setPlayersInAllIn(
    const std::vector< Notification::PlayerInAllInState >& playersInAllIn )
{
    m_playersInAllIn = playersInAllIn;
}

void TableState::setSeatKickReason( IDX seatIdx, sylk::Notification::KickReason kickReason )
{
    findSeat( seatIdx ).setKickReason( kickReason );
}

bool TableState::hasSeatKickReason( IDX seatIdx )
{
    return findSeat( seatIdx ).getKickReason().has_value();
}

std::vector< Notification::PlayerOnShowdownState >& TableState::getPlayersOnShowdown()
{
    return m_playersOnShowdown;
}

std::vector< Notification::PlayerInAllInState >& TableState::getPlayersInAllIn()
{
    return m_playersInAllIn;
}

Notification::Seat& TableState::findSeat( IDX seatIdx )
{
    auto it = std::find_if( m_seats.begin(), m_seats.end(), [ seatIdx ]( auto& seat ) {
        return seat.getSeatIdx() == seatIdx;
    } );

    if ( it != m_seats.end() )
    {
        return *it;
    }
    else
    {
        LOG( WARNING ) << "Seat with seatIdx: " << seatIdx << " was not found in table state";

        if ( !m_seats.empty() )
        {
            return m_seats.front();
        }
        else
        {
            return m_seats.emplace_back();
        }
    }
}

void TableState::createSeat( IDX seatIdx )
{
    m_seats.emplace_back().setSeatIdx( seatIdx );
}

void TableState::seatClearBet( IDX seatIdx )
{
    findSeat( seatIdx ).clearBet();
}

void TableState::increaseSeatStake( IDX seatIdx, uint64_t amount )
{
    findSeat( seatIdx ).increaseStake( amount );
}

uint64_t TableState::getActionTimeLimit() const
{
    // /1000 to send the result in seconds to the client
    return m_actionTimeLimit / 1000;
}

IDX TableState::getMySeatIdx() const
{
    return m_mySeatIdx;
}

void TableState::setMySeatIdx( IDX mySeatIdx )
{
    m_mySeatIdx = mySeatIdx;
}

bool TableState::getTimebankActivated() const
{
    return m_timebankActivated;
}

bool TableState::isSeatInSitout( IDX seatIdx )
{
    return findSeat( seatIdx ).inSitout();
}

void TableState::setSbAmount( Money sbAmount )
{
    m_sbAmount = sbAmount.getMoney();
}

IDX Seat::getSeatIdx() const
{
    return m_seatIdx;
}

uint64_t Seat::getStake() const
{
    return m_stake;
}

const std::string& Seat::getName() const
{
    return m_name;
}

uint64_t Seat::getBet() const
{
    return m_bet;
}

bool Seat::isFolded() const
{
    return m_isFolded;
}

bool Seat::isActionsAvailable() const
{
    return m_actionsOptions.has_value();
}

const std::vector< Card >& Seat::getCards() const
{
    return m_cards;
}

std::string Seat::getLastAction() const
{
    return m_lastAction;
}

const std::string& Seat::getUserAvatar() const
{
    return m_userAvatar;
}

std::string Seat::getLastPreAction() const
{
    return m_lastPreAction;
}

bool Seat::getMuckAvailability() const
{
    return m_canMuck;
}

bool Seat::canRebuy() const
{
    return m_canRebuy;
}

void Seat::setMuckAvailability( bool canMuck )
{
    m_canMuck = canMuck;
}

void Seat::setIsFolded( bool value )
{
    m_isFolded = value;
}

void Seat::setSeatIdx( IDX seatIdx )
{
    m_seatIdx = seatIdx;
}

void Seat::setName( const std::string& name )
{
    m_name = name;
}

void Seat::setCards( std::vector< Card > cards )
{
    m_cards = cards;
}

void Seat::setStake( uint64_t stake )
{
    m_stake = stake;
}

void Seat::setUserAvatar( const std::string& userAvatar )
{
    m_userAvatar = userAvatar;
}

void Seat::setLastAction( const std::string& lastAction )
{
    m_lastAction = lastAction;
}

void Seat::clearLastAction()
{
    m_lastAction.clear();
}

void Seat::setLastPreAction( const std::string& lastPreAction )
{
    m_lastPreAction = lastPreAction;
}

void Seat::clearLastPreAction()
{
    m_lastPreAction.clear();
}

void Seat::clearCards()
{
    m_cards.clear();
}

void Seat::addBet( uint64_t bet )
{
    m_bet += bet;
    m_stake -= bet;
}

void Seat::clearBet()
{
    m_bet = 0;
}

void Seat::increaseStake( uint64_t amount )
{
    m_stake += amount;
}

void Seat::setActionsOptions( sylk::ActionOptions actionOptions )
{
    m_actionsOptions.emplace( actionOptions );
}
void Seat::setPreActionsOptions( sylk::PreActionOptions preActionOptions )
{
    m_preActionsOptions.emplace( preActionOptions );
}

void Seat::clearActionsOptions()
{
    m_actionsOptions.reset();
}

bool Seat::isPlaying() const
{
    return m_isPlaying;
}

bool Seat::inSitout() const
{
    return m_inSitout;
}

void Seat::setIsPlaying( bool value )
{
    m_isPlaying = value;
}

void Seat::setInSitout( bool value )
{
    m_inSitout = value;
}

void Seat::setKickReason( KickReason kickReason )
{
    m_kickReason = kickReason;
}

std::optional< KickReason > Seat::getKickReason()
{
    return m_kickReason;
}

Pot::Pot( Money amount ) : m_amount( amount.getMoney() ) { }

Pot::Pot( Money amount, const std::vector< sylk::SidePot >& sidePots )
    : m_amount( amount.getMoney() )
    , m_sidePots( sidePots )
{
}

uint64_t Pot::getAmount() const
{
    return m_amount;
}

std::vector< sylk::SidePot > Pot::getSidePots() const
{
    return m_sidePots;
}

void Pot::setAmount( Money amount )
{
    m_amount = amount.getMoney();
}

void Pot::setSidePots( const std::vector< sylk::SidePot >& sidePots )
{
    m_sidePots = sidePots;
}

void Pot::reset()
{
    m_amount = 0;
    m_sidePots.clear();
}

Notification::SidePot::SidePot( Money amount, Money shortStack )
    : m_amount( amount.getMoney() )
    , m_shortStack( shortStack.getMoney() )
{
}

uint64_t Notification::SidePot::getAmount() const
{
    return m_amount;
}

uint64_t Notification::SidePot::getShortStack() const
{
    return m_shortStack;
}

Notification::SidePotShortStackPerAmount::SidePotShortStackPerAmount( Money amount,
                                                                      Money shortStack )
    : m_amount( amount.getMoney() )
    , m_shortStack( shortStack.getMoney() )
{
}

uint64_t Notification::SidePotShortStackPerAmount::getAmount() const
{
    return m_amount;
}

uint64_t Notification::SidePotShortStackPerAmount::getShortStack() const
{
    return m_shortStack;
}
void Seat::clearPreActionsOptions()
{
    m_preActionsOptions.reset();
}

void Seat::setCanRebuy( bool canRebuy )
{
    m_canRebuy = canRebuy;
}

bool Seat::isDisconnected() const
{
    return m_isDisconnected;
}

void Seat::setIsDisconnected( bool value )
{
    m_isDisconnected = value;
}

void Seat::clearDisconnect()
{
    m_isDisconnected = false;
}

std::vector< uint64_t > TableState::getSpinMultipliers() const
{
    return m_spinMultipliers;
}

void TableState::setSpinMultipliers( const std::vector< uint64_t >& spinMultipliers )
{
    m_spinMultipliers = spinMultipliers;
}

uint64_t TableState::getActiveSpinsMultiplier() const
{
    return m_activeSpinsMultiplier;
}

void TableState::setActiveSpinsMultiplier( uint64_t activeSpinsMultiplier )
{
    m_activeSpinsMultiplier = activeSpinsMultiplier;
}
