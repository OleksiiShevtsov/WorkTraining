#pragma once

#include "enum.h"

#include <map>
#include <string>

namespace sylk::common::notification
{
std::string to_string( NotificationTypes ntType )
{
    static const std::map< NotificationTypes, std::string > notificationType{
        { NotificationTypes::ntUserLoginToServer, "ntUserLoginToServer" },
        { NotificationTypes::ntLogoutFromServer, "ntLogoutFromServer" },
        { NotificationTypes::ntTableState, "ntTableState" },
        { NotificationTypes::ntSeatRequest, "ntSeatRequest" },
        { NotificationTypes::ntLeaveRequest, "ntLeaveRequest" },
        { NotificationTypes::ntCheckAction, "ntCheckAction" },
        { NotificationTypes::ntBetAction, "ntBetAction" },
        { NotificationTypes::ntRaiseAction, "ntRaiseAction" },
        { NotificationTypes::ntAllInAction, "ntAllInAction" },
        { NotificationTypes::ntCallAction, "ntCallAction" },
        { NotificationTypes::ntFoldAction, "ntFoldAction" },
        { NotificationTypes::ntPostBBAndPlay, "ntPostBBAndPlay" },
        { NotificationTypes::ntSitoutNextHand, "ntSitoutNextHand" },
        { NotificationTypes::ntSitoutNextHandAllTables, "ntSitoutNextHandAllTables" },
        { NotificationTypes::ntSitoutNextBB, "ntSitoutNextBB" },
        { NotificationTypes::ntSitoutNextBBAllTables, "ntSitoutNextBBAllTables" },
        { NotificationTypes::ntSitoutPlayerBack, "ntSitoutPlayerBack" },
        { NotificationTypes::ntWaitUserTimeout, "ntWaitUserTimeout" },
        { NotificationTypes::ntDelayTimeout, "ntDelayTimeout" },
        { NotificationTypes::ntMuckCards, "ntMuckCards" },
        { NotificationTypes::ntFoldToAnyBetPreAction, "ntFoldToAnyBetPreAction" },
        { NotificationTypes::ntRebuyRequest, "ntRebuyRequest" },
        { NotificationTypes::ntCheckFoldPreAction, "ntCheckFoldPreAction" },
        { NotificationTypes::ntUpdatePlayerSettings, "ntUpdatePlayerSettings" },
        { NotificationTypes::ntTableLobbyInfo, "ntTableLobbyInfo" },
        { NotificationTypes::ntDisconnect, "ntStartDisconnect" },
        { NotificationTypes::ntReconnect, "ntStopDisconnect" },
        { NotificationTypes::ntObserverRequest, "ntObserverRequest" },
        { NotificationTypes::ntPlayerBalance, "ntPlayerBalance" },
        { NotificationTypes::ntWaitlistRequestBuyin, "ntWaitlistRequestBuyin" },
        { NotificationTypes::ntWaitlistResponseBuyin, "ntWaitlistResponseBuyin" }
    };

    return notificationType.at( ntType );
}
} // namespace sylk::common::notification
