#pragma once

#include "core/type.h"
#include "core/db_connector.h"

#include <QTreeWidget>

namespace gnc
{
    class OperatorModelTree : public QTreeWidget
    {
    public:
        OperatorModelTree(QWidget* parent = nullptr);

        void loadData();
        void showTree();

    private:
        type::CountriesMobile m_countriesMobile;
        std::shared_ptr<DBConnector> m_db;
    };
}
