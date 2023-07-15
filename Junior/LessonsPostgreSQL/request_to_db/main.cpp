#include <iostream>
#include <sqlpp11/mysql/mysql.h>
#include <sqlpp11/sqlpp11.h>

int main()
{
    auto config = std::make_shared<sqlpp::mysql::connection_config>();
    config->user = "root";
    config->database = "sqlpp_mysql";
    config->debug = true;

    sqlpp::mysql::connection db(config);
}