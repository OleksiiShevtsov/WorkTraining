#include "notifications/notification_factory.h"
#include "common/notification/enum.h"

//#include <g3log/g3log.hpp>

#include <BetAction.pb.h>
#include <TableState.pb.h>

using namespace bof;
using namespace bof::common;

std::optional< NotificationVariant >
NotificationFactory::getNotificationFromProtobuf( const std::string& protoString )
{
    NotificationTypeMoc typeMoc;
    typeMoc.ParseFromString( protoString );
    const auto type = static_cast< NotificationTypes >( typeMoc.nt_type() );

    switch ( type )
    {
        case NotificationTypes::ntSeatRequest:
        {
            SeatRequest proto;
            proto.ParseFromString( protoString );

            return Notification::SeatRequest{ proto.token(),
                                              proto.table_id(),
                                              proto.buyin_amount() };
        }
        case NotificationTypes::ntLeaveRequest:
        {
            LeaveRequest proto;
            proto.ParseFromString( protoString );

            return Notification::LeaveRequest{ proto.token(), proto.table_id() };
        }
        case NotificationTypes::ntCheckAction:
        {
            CheckAction proto;
            proto.ParseFromString( protoString );

            return Notification::CheckAction{ proto.token(), proto.table_id() };
        }
        case NotificationTypes::ntBetAction:
        {
            BetAction proto;
            proto.ParseFromString( protoString );

            return Notification::BetAction{ proto.token(), proto.table_id(), proto.bet_amount() };
        }
        case NotificationTypes::ntRaiseAction:
        {
            RaiseAction proto;
            proto.ParseFromString( protoString );

            return Notification::RaiseAction{ proto.token(), proto.table_id(), proto.bet_amount() };
        }
        case NotificationTypes::ntAllInAction:
        {
            AllInAction proto;
            proto.ParseFromString( protoString );

            return Notification::AllInAction{ proto.token(), proto.table_id() };
        }
        case NotificationTypes::ntCallAction:
        {
            CallAction proto;
            proto.ParseFromString( protoString );

            return Notification::CallAction{ proto.token(), proto.table_id() };
        }
        case NotificationTypes::ntFoldAction:
        {
            FoldAction proto;
            proto.ParseFromString( protoString );

            return Notification::FoldAction{ proto.token(), proto.table_id() };
        }
        case NotificationTypes::ntMuckCards:
        {
            MuckCardsState proto;
            proto.ParseFromString( protoString );

            return Notification::MuckCards{ proto.token(), proto.table_id(), proto.muck() };
        }
        case NotificationTypes::ntPostBBAndPlay:
        {
            PostBBAndPlay proto;
            proto.ParseFromString( protoString );

            return Notification::PostBBAndPlay{ proto.token(),
                                                proto.table_id(),
                                                proto.is_active() };
        }
        case NotificationTypes::ntRebuyRequest:
        {
            RebuyRequest proto;
            proto.ParseFromString( protoString );

            return Notification::RebuyRequest{ proto.token(), proto.table_id(), proto.amount() };
        }
        case NotificationTypes::ntFoldToAnyBetPreAction:
        {
            FoldToAnyBetPreAction proto;
            proto.ParseFromString( protoString );

            return Notification::FoldBetPreAction{ proto.token(),
                                                   proto.table_id(),
                                                   proto.is_active() };
        }
        case NotificationTypes::ntCheckFoldPreAction:
        {
            CheckFoldPreAction proto;
            proto.ParseFromString( protoString );

            return Notification::CheckFoldPreAction{ proto.token(),
                                                     proto.table_id(),
                                                     proto.is_active() };
        }
        case NotificationTypes::ntSitoutNextHand:
        {
            SitoutNextHand proto;
            proto.ParseFromString( protoString );

            return Notification::SitoutNextHand{ proto.token(),
                                                 proto.table_id(),
                                                 proto.is_active() };
        }
        case NotificationTypes::ntSitoutNextHandAllTables:
        {
            SitoutNextHandAllTables proto;
            proto.ParseFromString( protoString );

            return Notification::SitoutNextHandAllTables{ proto.token(), proto.is_active() };
        }
        case NotificationTypes::ntSitoutNextBB:
        {
            SitoutNextBB proto;
            proto.ParseFromString( protoString );

            return Notification::SitoutNextBB{ proto.token(), proto.table_id(), proto.is_active() };
        }
        case NotificationTypes::ntSitoutNextBBAllTables:
        {
            SitoutNextBBAllTables proto;
            proto.ParseFromString( protoString );

            return Notification::SitoutNextBBAllTables{ proto.token(), proto.is_active() };
        }
        case NotificationTypes::ntSitoutPlayerBack:
        {
            SitoutPlayerBack proto;
            proto.ParseFromString( protoString );

            return Notification::SitoutPlayerBack{ proto.token(), proto.table_id() };
        }
        case NotificationTypes::ntUpdatePlayerSettings:
        {
            UpdatePlayerSettings proto;
            proto.ParseFromString( protoString );

            return Notification::UpdatePlayerSettings{ proto.token() };
        }
        case NotificationTypes::ntUserLoginToServer:
        {
            LoginToServer proto;
            proto.ParseFromString( protoString );

            return Notification::UserLogin{ proto.token() };
        }
        case NotificationTypes::ntObserverRequest:
        {
            ObserverRequest proto;
            proto.ParseFromString( protoString );

            return Notification::ObserverRequest( proto.token(), proto.table_id() );
        }
        case NotificationTypes::ntWaitlistResponseBuyin:
        {
            WaitlistResponseBuyin proto;
            proto.ParseFromString( protoString );

            return Notification::WaitlistResponseBuyin{ proto.token(),
                                                        proto.table_id(),
                                                        proto.buyin_amount(),
                                                        proto.declined() };
        }
        default:
        {
            LOG( WARNING ) << "Notification type was not detected";
        }
    }

    return std::nullopt;
}

std::optional< NotificationVariant >

NotificationFactory::createNotification( NotificationTypes notificationType, ID playerId )
{
    switch ( notificationType )
    {
        case NotificationTypes::ntDisconnect:
        {
            return Notification::Disconnect{ playerId };
        }
        case NotificationTypes::ntReconnect:
        {
            return Notification::Reconnect{ playerId };
        }
        default:
        {
            LOG( INFO ) << "Unhandled creation of notification";
        }
    }

    return std::nullopt;
}

std::string NotificationFactory::getProtoStringFromNotification( NotificationVariant notification )
{
    return std::visit( NotificationFactory::NotificationHandler{}, notification );
}

std::string NotificationFactory::NotificationHandler::operator()(
    Notification::TableState& notificationTableState )
{
    TableState tableState;

    tableState.set_nt_type( NotificationTypes::ntTableState );
    tableState.set_table_id( notificationTableState.getTableId().value() );
    tableState.set_table_name( notificationTableState.getTableName() );
    tableState.set_bb_amount( notificationTableState.getBbAmount() );
    tableState.set_sb_amount( notificationTableState.getSbAmount() );
    tableState.set_seat_idx_bb( notificationTableState.getSeatIdxBb() );
    tableState.set_seat_idx_sb( notificationTableState.getSeatIdxSb() );
    tableState.set_seat_idx_btn( notificationTableState.getSeatIdxButton() );
    tableState.set_my_seat_idx( notificationTableState.getMySeatIdx() );
    tableState.set_game_state( static_cast< GameState >( notificationTableState.getGameState() ) );
    tableState.set_action_time_limit( notificationTableState.getActionTimeLimit() );
    tableState.set_timebank_activated( notificationTableState.getTimebankActivated() );

    auto protoPotInfo = new ::PotInfo;
    auto customPot    = notificationTableState.getPot();
    protoPotInfo->set_amount( customPot.getAmount() );
    if ( !notificationTableState.getPot().getSidePots().empty() )
    {
        for ( const auto& sidePot : notificationTableState.getPot().getSidePots() )
        {
            auto protoSidePot = protoPotInfo->add_side_pots();
            protoSidePot->set_amount( sidePot.getAmount().getMoney() );
            protoSidePot->set_short_stack( sidePot.getShortStack().getMoney() );
        }
    }
    tableState.set_allocated_pot( protoPotInfo );

    if ( !notificationTableState.getSpinMultipliers().empty() )
    {
        for ( auto spinMultiplier : notificationTableState.getSpinMultipliers() )
        {
            tableState.add_spin_multipliers( spinMultiplier );
        }
        tableState.set_active_spins_multiplier( notificationTableState.getActiveSpinsMultiplier() );
    }

    if ( !notificationTableState.getShowdownState().getWinners().empty() )
    {
        auto showdownState = tableState.add_showdown_state();

        auto protoHandPlayed{ new ::HandPlayedInfo{} };
        protoHandPlayed->set_file_name(
            notificationTableState.getShowdownState().getHandPlayedInfo().getFileName() );
        protoHandPlayed->set_mongo_hand_id(
            notificationTableState.getShowdownState().getHandPlayedInfo().getHandMongoId() );
        protoHandPlayed->set_hand_header(
            notificationTableState.getShowdownState().getHandPlayedInfo().getHandHeader() );
        protoHandPlayed->set_hand_body(
            notificationTableState.getShowdownState().getHandPlayedInfo().getHandBody() );

        showdownState->set_allocated_hand_played_info( protoHandPlayed );

        for ( const auto& winner : notificationTableState.getShowdownState().getWinners() )
        {
            auto protoWinner = showdownState->add_winners();
            protoWinner->set_seat_idx( winner.getSeatIdx() );
            protoWinner->set_money_reward( winner.getMoneyReward() );

            const auto sidePotShortStackPerAmount = winner.getSidePotShortStackPerAmount();
            if ( !sidePotShortStackPerAmount.empty() )
            {
                for ( const auto item : sidePotShortStackPerAmount )
                {
                    auto protoSidePotShortStacks = protoWinner->add_side_pots_short_stacks();
                    protoSidePotShortStacks->set_short_stack( item.getShortStack() );
                    protoSidePotShortStacks->set_amount( item.getAmount() );
                }
            }
        }

        for ( const auto& playerShowdown :
              notificationTableState.getShowdownState().getShowdownPlayers() )
        {
            auto protoPlayerShowdown = showdownState->add_player_on_showdown();
            protoPlayerShowdown->set_seat_idx( playerShowdown.getSeatIdx() );
            protoPlayerShowdown->set_muck( playerShowdown.getMuck() );

            if ( !playerShowdown.getCards().empty() )
            {
                for ( const auto& card : playerShowdown.getCards() )
                {
                    protoPlayerShowdown->add_cards( card );
                }
            }
        }
    }

    if ( !notificationTableState.getCardsShowState().getPlayersInAllIn().empty() )
    {
        auto cardsShowState = tableState.add_cards_show_state();

        for ( const auto& playerInAllIn : notificationTableState.getPlayersInAllIn() )
        {
            auto protoPlayerInAllIn = cardsShowState->add_player_in_allin();
            protoPlayerInAllIn->set_seat_idx( playerInAllIn.getSeatIdx() );

            if ( !playerInAllIn.getCards().empty() )
            {
                for ( const auto& card : playerInAllIn.getCards() )
                {
                    protoPlayerInAllIn->add_cards( card );
                }
            }
            protoPlayerInAllIn->set_win_percentage( playerInAllIn.getWinPercentage() );
        }
    }

    for ( const auto& card : notificationTableState.getCommunityCards() )
    {
        tableState.add_community_cards( card );
    }

    for ( auto& seat : notificationTableState.getSeats() )
    {
        auto protoSeat = tableState.add_seats();
        protoSeat->set_seat_idx( seat.getSeatIdx() );
        protoSeat->set_is_folded( seat.isFolded() );
        protoSeat->set_is_playing( seat.isPlaying() );
        protoSeat->set_is_actions_available( seat.isActionsAvailable() );
        protoSeat->set_stake( seat.getStake() );
        protoSeat->set_bet( seat.getBet() );
        protoSeat->set_user_avatar( seat.getUserAvatar() );
        protoSeat->set_name( seat.getName() );
        protoSeat->set_can_muck( seat.getMuckAvailability() );
        protoSeat->set_is_disconnected( seat.isDisconnected() );
        protoSeat->set_in_sitout( seat.inSitout() );
        protoSeat->set_can_rebuy( seat.canRebuy() );

        if ( const auto kickReason{ seat.getKickReason() }; kickReason.has_value() )
        {
            protoSeat->set_kick_reason( static_cast<::KickReason >( kickReason.value() ) );
        }

        if ( seat.getPreActionsOptions().has_value() )
        {
            auto protoPreActionOptions = new ::PreActionOptionsState();

            const auto preActionOptions = seat.getPreActionsOptions();

            for ( const auto& preAction : preActionOptions->getSeatsPreActions() )
            {
                protoPreActionOptions->add_seat_pre_actions(
                    static_cast< PreActions >( preAction ) );
            }

            protoSeat->set_allocated_pre_action_options( protoPreActionOptions );
        }

        if ( const auto lastAction{ seat.getLastAction() }; !lastAction.empty() )
        {
            protoSeat->set_last_action( lastAction );
        }

        if ( const auto lastPreAction{ seat.getLastPreAction() }; !lastPreAction.empty() )
        {
            protoSeat->set_last_pre_action( lastPreAction );
        }

        if ( !seat.getCards().empty() )
        {
            for ( const auto& card : seat.getCards() )
            {
                protoSeat->add_cards( card.toString() );
            }
        }

        if ( seat.getActionsOptions().has_value() )
        {
            auto protoActionOptions = new ::ActionOptionsState;

            const auto actionOptions = seat.getActionsOptions();
            protoActionOptions->set_max_bet( actionOptions->getMaxBet() );
            protoActionOptions->set_min_bet( actionOptions->getMinBet() );

            for ( const auto& action : actionOptions->getSeatsActions() )
            {
                protoActionOptions->add_seat_actions( static_cast< SeatActions >( action ) );
            }

            protoSeat->set_allocated_action_options( protoActionOptions );
        }
    }

    LOG( INFO ) << "Notification: ntTableState was sent";

    return tableState.SerializeAsString();
}

std::string NotificationFactory::NotificationHandler::operator()(
    Notification::TablesLobbyInfo& tablesLobbyInfo )
{
    TablesLobbyInfo lobbyInfo;
    lobbyInfo.set_nt_type( ntTableLobbyInfo );

    for ( const auto& tableInfo : tablesLobbyInfo.getTablesInfo() )
    {
        auto protoTableInfo = lobbyInfo.add_tables_info();
        protoTableInfo->set_table_id( tableInfo.m_tableId );
        protoTableInfo->set_name( tableInfo.m_name );
        protoTableInfo->set_waiting_list_size( tableInfo.m_waitingListSize );
        protoTableInfo->set_occupied_seats( tableInfo.m_occupiedSeats );
        protoTableInfo->set_max_seats_size( tableInfo.m_maxSeats );
        protoTableInfo->set_small_blind( tableInfo.m_smallBlind );
        protoTableInfo->set_big_blind( tableInfo.m_bigBlind );
        protoTableInfo->set_stack( tableInfo.m_stack );

        for ( const auto& playerData : tableInfo.getlistPlayersAtTable() )
        {
            auto protoPlayerData = protoTableInfo->add_players_data();
            protoPlayerData->set_seat_id( playerData.m_seatIdx );
            protoPlayerData->set_name( playerData.m_name );
            protoPlayerData->set_stack( playerData.m_stake );
        }
    }

    LOG( INFO ) << "Notification: ntTableLobbyInfo was sent";

    return lobbyInfo.SerializeAsString();
}

std::string
NotificationFactory::NotificationHandler::operator()( Notification::PlayerBalance& playerBalance )
{
    Balance protoPlayerBalance;
    protoPlayerBalance.set_nt_type( NotificationTypes::ntPlayerBalance );
    protoPlayerBalance.set_player_balance( playerBalance.getPlayerBalance() );

    LOG( INFO ) << "Notification: ntPlayerBalance was sent";

    return protoPlayerBalance.SerializeAsString();
}

std::string NotificationFactory::NotificationHandler::operator()(
    Notification::WaitlistRequestBuyin& waitlistRequestBuyin )
{
    WaitlistRequestBuyin proto;
    proto.set_nt_type( ntWaitlistRequestBuyin );
    proto.set_table_id( waitlistRequestBuyin.getTableId().value() );

    const auto& blinds{ waitlistRequestBuyin.getBlinds() };
    proto.set_small_blind( blinds.getSmall().getMoney() );
    proto.set_big_blind( blinds.getBig().getMoney() );
    proto.set_stake( waitlistRequestBuyin.getStake() );

    LOG( INFO ) << "Notification: ntWaitlistRequestBuyin was sent";

    return proto.SerializeAsString();
}
