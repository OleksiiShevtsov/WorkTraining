#include "core/db_connector.h"
#include <iostream>

gnc::DBConnector::DBConnector(const std::string& dbFile)
{
    if (sqlite3_open(dbFile.c_str(), &m_db) != SQLITE_OK)
    {
        std::cerr << "Error opening database: " << sqlite3_errmsg(m_db) << std::endl;
    }
}

gnc::DBConnector::~DBConnector()
{
    if(m_db)
    {
        sqlite3_close(m_db);
    }
}

gnc::type::QueryResult gnc::DBConnector::selectQuery(const std::string& query)
{
    type::QueryResult results;
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error("Failed to execute query: " + std::string(sqlite3_errmsg(m_db)));
    }

    int columnCount = sqlite3_column_count(stmt);
    
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        std::vector<std::string> row;

        for (size_t i{}; i < columnCount; ++i)
        {
            const char* columnText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            row.push_back(columnText ? columnText : "");
        }
        results.push_back(row);
    }

    sqlite3_finalize(stmt);
    return results;
}
