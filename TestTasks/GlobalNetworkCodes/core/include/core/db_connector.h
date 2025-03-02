#pragma once

#include "core/type.h"

#include <sqlite3.h>
#include <string>

namespace gnc
{
    class DBConnector
    {
    public:

        DBConnector(const std::string& dbFile);
        ~DBConnector();

        type::QueryResult selectQuery(const std::string& query);

    private:
        sqlite3* m_db = nullptr;
    };
};
