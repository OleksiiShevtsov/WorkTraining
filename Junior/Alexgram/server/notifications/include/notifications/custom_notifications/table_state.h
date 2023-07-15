#pragma once

#include "common/types.h"
#include "logic/action_options.h"
#include "logic/card.h"
#include "logic/enums/preaction.h"
#include "logic/enums/seat_action.h"
#include "logic/money.h"
#include "logic/preaction_options.h"
#include "logic/side_pot.h"
#include "logic/typedefs.h"
#include "notifications/notification.h"

#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace sylk
{
namespace Notification
{
enum class GameState
{
    Waiting           = 0,
    PreFlop           = 1,
    Flop              = 2,
    Turn              = 3,
    River             = 4,
    Muck              = 5,
    Showdown          = 6,
    Closed            = 7,
    Default           = 8,
    PreparingNextHand = 9
};

enum class SeatActions
{
    PostSB = 0,
    PostBB = 1,
    Bet    = 2,
    Raise  = 3,
    AllIn  = 4,
    Fold   = 5,
    Check  = 6,
    Call   = 7
};

enum class PreActions
{
    NoPreaction   = 0,
    pFoldToAnyBet = 1,
    pCheck        = 2,
    pCheckOrFold  = 3,
    pCall         = 4
};

enum class KickReason
{
    SitoutLimit = 0
};

class ActionsOptions
{
public:
    ActionsOptions( sylk::ActionOptions actionOptions );

    int64_t getMinBet() const;
    int64_t getMaxBet() const;
    std::vector< sylk::SeatAction > getSeatsActions() const;

private:
    int64_t m_minBet = 0;
    int64_t m_maxBet = 0;
    std::vector< sylk::SeatAction > m_seatActions;
};

class PreActionsOptions
{
public:
    PreActionsOptions( sylk::PreActionOptions preActionOptions );

    std::vector< sylk::PreAction > getSeatsPreActions() const;

private:
    std::vector< sylk::PreAction > m_seatPreActions;
};

class PlayerOnShowdownState
{
public:
    PlayerOnShowdownState( common::IDX seatIdx,
                           bool muck,
                           const std::vector< std::string >& cards );

    common::IDX getSeatIdx() const;
    bool getMuck() const;
    std::vector< std::string > getCards() const;
    void setSeatIdx( common::IDX seatIdx );
    void setMuck( bool muck );
    void setCards( const std::vector< std::string >& cards );

private:
    common::IDX m_seatIdx;
    bool m_muck;
    std::vector< std::string > m_cards;
};

class PlayerInAllInState
{
public:
    PlayerInAllInState( common::ID seatIdx, const std::vector< std::string >& cards );

    common::IDX getSeatIdx() const;
    std::vector< std::string > getCards() const;
    double getWinPercentage() const;
    void setSeatIdx( common::IDX seatIdx );
    void setCards( const std::vector< std::string >& cards );
    void setWinPercentage( double winPercentage );

private:
    common::IDX m_seatIdx;
    std::vector< std::string > m_cards;
    double m_winPercentage{ 0 };
};

class SidePotShortStackPerAmount
{
public:
    SidePotShortStackPerAmount( Money amount, Money shortStack );

    uint64_t getAmount() const;
    uint64_t getShortStack() const;

private:
    uint64_t m_amount     = 0;
    uint64_t m_shortStack = 0;
};

class WinnerState
{
public:
    WinnerState( common::IDX seatIdx, uint64_t moneyReward );
    WinnerState( common::IDX seatIdx,
                 uint64_t moneyReward,
                 const std::vector< SidePotReward >& sidePotShortStackPerAmount );

    common::IDX getSeatIdx() const;
    uint64_t getMoneyReward() const;
    std::vector< SidePotShortStackPerAmount > getSidePotShortStackPerAmount() const;

private:
    common::IDX m_seatIdx;
    uint64_t m_moneyReward;
    std::vector< SidePotShortStackPerAmount > m_sidePotShortStackPerAmount;
};

class SidePot
{
public:
    SidePot( Money amount, Money shortStack );

    uint64_t getAmount() const;
    uint64_t getShortStack() const;

private:
    uint64_t m_amount     = 0;
    uint64_t m_shortStack = 0;
};

class Pot
{
public:
    Pot() = default;
    Pot( Money amount );
    Pot( Money amount, const std::vector< sylk::SidePot >& sidePots );

    uint64_t getAmount() const;
    std::vector< sylk::SidePot > getSidePots() const;

    void setAmount( Money amount );
    void setSidePots( const std::vector< sylk::SidePot >& sidePots );

    void reset();

private:
    uint64_t m_amount = 0;
    std::vector< sylk::SidePot > m_sidePots;
};

class CardsShowState
{
public:
    CardsShowState() = default;
    CardsShowState( const std::vector< PlayerInAllInState >& playerInAllIn );

    std::vector< PlayerInAllInState > getPlayersInAllIn() const;
    void reset();

private:
    std::vector< PlayerInAllInState > m_playerInAllIn;
};

class HandPlayedInfo
{
public:
    HandPlayedInfo() = default;
    HandPlayedInfo( const std::string& fileName,
                    const std::string& mongoHandId,
                    const std::string& handHeader,
                    const std::string& handBody );

    const std::string& getFileName() const;
    const std::string& getHandMongoId() const;
    const std::string& getHandHeader() const;
    const std::string& getHandBody() const;

private:
    std::string m_fileName;
    std::string m_handMongoId;
    std::string m_handHeader;
    std::string m_handBody;
};

class ShowdownState
{
public:
    ShowdownState() = default;
    ShowdownState( const std::vector< WinnerState >& winners,
                   const std::vector< PlayerOnShowdownState >& playerOnShowdown,
                   HandPlayedInfo handPlayedInfo );

    std::vector< WinnerState > getWinners() const;
    std::vector< PlayerOnShowdownState > getShowdownPlayers() const;
    HandPlayedInfo getHandPlayedInfo() const;

    void reset();

private:
    std::vector< WinnerState > m_winners;
    std::vector< PlayerOnShowdownState > m_playerOnShowdown;
    HandPlayedInfo m_handPlayedInfo;
};

class Seat
{
public:
    common::IDX getSeatIdx() const;
    bool isFolded() const;
    bool isPlaying() const;
    bool inSitout() const;

    uint64_t getStake() const;
    const std::string& getName() const;
    const std::string& getUserAvatar() const;
    uint64_t getBet() const;
    bool isActionsAvailable() const;
    const std::vector< Card >& getCards() const;
    std::string getLastAction() const;
    std::string getLastPreAction() const;
    std::optional< Notification::ActionsOptions > getActionsOptions() const;
    std::optional< Notification::PreActionsOptions > getPreActionsOptions() const;
    bool isDisconnected() const;
    bool getMuckAvailability() const;
    bool canRebuy() const;

    void setMuckAvailability( bool canMuck );
    void setIsFolded( bool value );
    void setIsPlaying( bool value );
    void setInSitout( bool value );
    void setSeatIdx( common::IDX seatIdx );
    void setName( const std::string& name );
    void setUserAvatar( const std::string& userAvatar );
    void addBet( uint64_t bet );
    void clearBet();
    void increaseStake( uint64_t amount );
    void setCards( std::vector< Card > cards );
    void clearCards();
    void setStake( uint64_t stake );
    void setIsDisconnected( bool value );
    void clearDisconnect();
    void setLastAction( const std::string& lastAction );
    void setLastPreAction( const std::string& lastPreAction );
    void clearLastAction();
    void clearLastPreAction();
    void setActionsOptions( sylk::ActionOptions actionOptions );
    void setPreActionsOptions( sylk::PreActionOptions preActionOptions );
    void clearActionsOptions();
    void clearPreActionsOptions();
    void setCanRebuy( bool canRebuy );

    // When this setted player will be kicked from server
    // Starting from the next hand
    void setKickReason( KickReason kickReason );
    std::optional< KickReason > getKickReason();

private:
    common::IDX m_seatIdx = 0;
    bool m_isFolded{ true };
    bool m_isPlaying{ false };
    bool m_isDisconnected{ false };
    bool m_inSitout{ false };
    uint64_t m_stake{};
    std::string m_name;
    std::string m_userAvatar;
    uint64_t m_bet = 0;
    std::string m_lastAction;
    std::string m_lastPreAction;
    std::vector< Card > m_cards;
    std::optional< ActionsOptions > m_actionsOptions;
    std::optional< PreActionsOptions > m_preActionsOptions;
    std::optional< KickReason > m_kickReason;
    bool m_canRebuy{ false };
    bool m_canMuck{ false };
};

class TableState : public INotification
{
public:
    TableState( common::ID tableId );
    TableState( common::ID tableId,
                const std::string& tableName,
                common::IDX seatIdxBB,
                common::IDX seatIdxSB,
                uint64_t bbAmount,
                uint64_t sbAmount,
                GameState gameState,
                const std::vector< std::string >& communityCards,
                const std::vector< Seat >& seats,
                ShowdownState showdownState );

    void setPlayerId( common::ID playerId );
    // TODO: Attention! This setter and member need as a key for websocket server know, which client
    // must get this notification. Need to be changed by token and reworked.

    void setTableName( const std::string& tableName );
    void setSeatIdxBb( common::IDX seatIdxBb );
    void setSeatIdxSb( common::IDX seatIdxSb );
    void setSeatIdxButton( common::IDX seatIdxButton );
    void setMySeatIdx( common::IDX mySeatIdx );

    void setPot( const Pot& pot );
    void setBbAmount( Money bbAmount );
    void setSbAmount( Money sbAmount );
    void setGameState( GameState gameState );
    void setShowdownState( ShowdownState showdownState );
    void setCardsShowState( CardsShowState cardsShowState );
    void setSeatActionTimeLimit( std::chrono::milliseconds actionTimeLimit );
    void setTimebankActivated( bool timebankActivated );

    common::ID getPlayerId() const override;

    std::optional< common::ID > getTableId() const override;
    std::string getTableName() const;
    common::IDX getSeatIdxBb() const;
    common::IDX getSeatIdxSb() const;
    common::IDX getSeatIdxButton() const;
    common::IDX getMySeatIdx() const;
    uint64_t getActionTimeLimit() const;
    bool getTimebankActivated() const;
    std::vector< Notification::PlayerOnShowdownState >& getPlayersOnShowdown();
    std::vector< Notification::PlayerInAllInState >& getPlayersInAllIn();

    Pot getPot() const;
    uint64_t getBbAmount() const;
    uint64_t getSbAmount() const;
    GameState getGameState() const;

    std::vector< Seat >& getSeats();
    std::vector< std::string >& getCommunityCards();
    ShowdownState getShowdownState() const;
    CardsShowState getCardsShowState() const;
    void reset();

    // Functions for seats

    void createSeat( common::IDX seatIdx );

    void setSeatIsFolded( common::IDX seatIdx, bool value );
    void setSeatIsPlaying( common::IDX seatIdx, bool value );
    void setSeatMuckAvailability( common::IDX seatIdx, bool value );
    void setCanRebuy( common::IDX seatIdx, bool value );
    void setSeatInSitout( common::IDX seatIdx, bool value );
    void setSeatSeatIdx( common::IDX seatIdx );
    void setSeatName( common::IDX seatIdx, std::string name );
    void setSeatAvatar( common::IDX seatIdx, const std::string& avatar );
    void addSeatBet( common::IDX seatIdx, uint64_t bet );
    void increaseSeatStake( common::IDX seatIdx, uint64_t amount );
    void seatClearBet( common::IDX seatIdx );
    void setSeatCards( common::IDX seatIdx, std::vector< Card > cards );
    void seatClearCards( common::IDX seatIdx );
    void setSeatStake( common::IDX seatIdx, uint64_t stake );
    void setSeatLastAction( common::IDX seatIdx, SeatAction action );
    void setSeatLastPreAction( common::IDX seatIdx, PreAction preAction );
    void seatClearLastAction( common::IDX seatIdx );
    void seatClearLastPreAction( common::IDX seatIdx );
    void clearAllSeatsLastAction();
    void clearAllSeatsLastPreAction();
    void setSeatActionOptions( common::IDX seatIdx, const sylk::ActionOptions& actionOptions );
    void setSeatPreActionOptions( common::IDX seatIdx,
                                  const sylk::PreActionOptions& preActionOptions );
    void setSeatIsDisconnected( common::IDX seatIdx, bool value );
    void clearSeatState( common::IDX seatIdx );
    void setPlayersOnShowdown(
        const std::vector< Notification::PlayerOnShowdownState >& playersOnShowdown );
    void setPlayersInAllIn( const std::vector< Notification::PlayerInAllInState >& playersInAllIn );
    bool isSeatInSitout( common::IDX seatIdx );

    // When this setted player will be kicked from server
    // Starting from the next hand
    void setSeatKickReason( common::IDX seatIdx, sylk::Notification::KickReason kickReason );
    bool hasSeatKickReason( common::IDX seatIdx );

    std::vector< uint64_t > getSpinMultipliers() const;
    void setSpinMultipliers( const std::vector< uint64_t >& spinMultipliers );

    uint64_t getActiveSpinsMultiplier() const;
    void setActiveSpinsMultiplier( uint64_t activeSpinsMultiplier );

private:
    Notification::Seat& findSeat( common::IDX seatIdx );

    common::ID m_playerId = 0;
    common::ID m_tableId  = 0;
    std::string m_tableName;
    common::IDX m_seatIdxBB     = 0;
    common::IDX m_seatIdxSB     = 0;
    common::IDX m_seatIdxButton = 0;
    common::IDX m_mySeatIdx     = 0;
    Pot m_pot;
    uint64_t m_bbAmount = 0;
    uint64_t m_sbAmount = 0;

    GameState m_gameState{ GameState::Default };
    std::vector< std::string > m_communityCards;
    std::vector< Seat > m_seats;
    std::vector< Notification::PlayerOnShowdownState > m_playersOnShowdown;
    std::vector< Notification::PlayerInAllInState > m_playersInAllIn;
    ShowdownState m_showdownState;
    CardsShowState m_cardsShowState;
    uint64_t m_actionTimeLimit = 0;
    bool m_timebankActivated   = false;
    std::vector< uint64_t > m_spinMultipliers{};
    uint64_t m_activeSpinsMultiplier{};
};

} // namespace Notification
} // namespace sylk
