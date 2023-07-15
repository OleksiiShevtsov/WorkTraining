#pragma once

namespace sylk::common
{
enum NotificationTypes
{
    ntUserLoginToServer,
    ntLogoutFromServer,
    ntTableState,
    ntSeatRequest,
    ntLeaveRequest,
    ntCheckAction,
    ntBetAction,
    ntRaiseAction,
    ntAllInAction,
    ntCallAction,
    ntFoldAction,
    ntPostBBAndPlay,
    ntSitoutNextHand,
    ntSitoutNextHandAllTables,
    ntSitoutNextBB,
    ntSitoutNextBBAllTables,
    ntSitoutPlayerBack,
    ntWaitUserTimeout,
    ntDelayTimeout,
    ntMuckCards,
    ntFoldToAnyBetPreAction,
    ntRebuyRequest,
    ntCheckFoldPreAction,
    ntUpdatePlayerSettings,
    ntTableLobbyInfo,
    ntObserverRequest,
    ntPlayerBalance,
    ntWaitlistRequestBuyin,
    ntWaitlistResponseBuyin,

    // Server only
    ntDisconnect,
    ntReconnect
};
}
