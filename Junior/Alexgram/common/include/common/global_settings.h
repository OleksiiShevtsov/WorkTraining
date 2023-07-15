#pragma once

#include <chrono>
#include <map>
#include <vector>

namespace sylk::common::GlobalSettings
{
using namespace std::chrono_literals;
constexpr auto projectName{ "Sylkpoker" };
constexpr auto omahaPokerName{ "Omaha" };
constexpr auto holdemPokerName{ "Holdem" };
constexpr auto restHostName{ "rest" };
constexpr auto mysqlHostName{ "mysql" };
constexpr auto mongoHostName{ "mongo" };
constexpr auto dbname{ "dbname" };
constexpr auto collectionName{ "collection" };
constexpr auto listHostsName{ "hosts" };
constexpr auto workUrl{ "workUrl" };
constexpr auto alternativeUrls{ "urls" };
constexpr auto username{ "username" };
constexpr auto password{ "password" };
constexpr auto adminName{ "admin" };
constexpr auto port{ "port" };
constexpr auto maxLoginIncorrectAttempts{ 3 };
constexpr auto configFileName{ "config.json" };
constexpr auto gameLimit{ "No Limit" };
constexpr auto handHistoryDir{ "HandHistory" };
constexpr size_t minPlayersToStart{ 2 };
constexpr size_t tournamentLimitForPlayers{ 3 };
constexpr size_t holdemHoleCardsCount{ 2 };
constexpr size_t omahaHoleCardsCount{ 4 };
constexpr std::chrono::milliseconds stateMachineNewHandDelay{ 4s };
constexpr std::chrono::milliseconds timePlayerAction{ 30s };
constexpr std::chrono::milliseconds disconnectionTimebankDelay{ 60s };
constexpr size_t tablesLimitForPlayer{ 6 };
constexpr double minAllowedByuinFraction{ 0.2 };
constexpr int replyTimeoutDuration{ 2 };
constexpr size_t maxWaitlistSize{ 100 };
constexpr double rakePercent{ 5.0 };
constexpr uint64_t defaultRakeCap{ 50 };
constexpr uint64_t surfaceFormat{ 8 };
const std::map< int64_t, uint64_t > rakeCapForBigBlind{ { 10, 50 },    { 20, 100 },  { 50, 200 },
                                                        { 100, 250 },  { 200, 300 }, { 500, 400 },
                                                        { 1000, 500 }, { 2000, 650 } };
struct HandHistoryDownloadDelay
{
    static constexpr auto hours{ 3 };
    static constexpr auto minutes{ 0 };
};

namespace Email
{
constexpr auto smtpServerAddress{ "smtp.gmail.com" };
constexpr auto smtpServerPort{ 587 };
constexpr auto serviceEmail{ "sylkpoker@gmail.com" };
constexpr auto serviceEmailPassword{ "letsplaypokertillnightQ" };
} // namespace Email
} // namespace sylk::common::GlobalSettings
