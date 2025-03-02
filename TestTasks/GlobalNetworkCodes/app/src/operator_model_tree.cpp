#include "app/operator_model_tree.h"
#include "app/styled_delegate_country_operator.h"

#include <QMessageBox>

gnc::OperatorModelTree::OperatorModelTree(QWidget* parent) :
    QTreeWidget(parent)
{
    m_db = std::make_shared<DBConnector>("OM system.db");

    StyledDelegateCountryOperator* styledDelegateCountryOperator = new StyledDelegateCountryOperator(this);
    this->setItemDelegateForColumn(0, styledDelegateCountryOperator);
}

void gnc::OperatorModelTree::loadData()
{
    std::string query = R"(
            SELECT
            countries.mcc,
            countries.mnc_length,
            countries.code,
            countries.name,
            operators.mnc,
            operators.name
            FROM countries
            LEFT JOIN operators
            ON countries.mcc = operators.mcc;
        )";

    try
    {
        gnc::type::QueryResult queries = m_db->selectQuery(query);
    }
    catch (const std::runtime_error& e)
    {
        QMessageBox::critical(nullptr, "database query error!", e.what());
        return;
    }

    gnc::type::QueryResult queries = m_db->selectQuery(query);

    for (const auto& query : queries) {
        uint16_t mcc = static_cast<uint16_t>(std::stoi(query[0]));
        uint16_t mncLength = static_cast<uint16_t>(std::stoi(query[1]));
        std::string code = query[2];
        std::string countryName = query[3];

        if (!m_countriesMobile.contains(code)) {
            gnc::CountryMobile country{mcc, mncLength, countryName, {}};
            m_countriesMobile[code] = country;
        }

        if (!query[4].empty()) {
            uint16_t mnc = static_cast<uint16_t>(std::stoi(query[4]));
            std::string operatorName = query[5];
            m_countriesMobile[code].operators.emplace_back(mnc, operatorName);
        }
    }
}

void gnc::OperatorModelTree::showTree()
{
    setHeaderLabel("Network Codes");

    for(const auto& countryMobile : m_countriesMobile)
    {
        QTreeWidgetItem* countryItem = new QTreeWidgetItem(this);
        std::string countryItemText = countryMobile.second.name + " (" + countryMobile.first + ")";
        std::string countryItemIcon = ":/png/countries/" + countryMobile.first + ".png";

        countryItem->setText(0, QString::fromStdString(countryItemText));
        countryItem->setData(0, Qt::UserRole, QString::fromStdString(countryItemIcon));

        for(const auto& oper : countryMobile.second.operators)
        {
            QTreeWidgetItem* operatorItem = new QTreeWidgetItem(countryItem);
            std::string operatorItemText = oper.name + " (" +
                                            std::to_string(countryMobile.second.mcc) + "-" +
                                            std::to_string(oper.mnc) + ")";

            std::string operatorItemIcon = ":/png/operators/" + std::to_string(countryMobile.second.mcc) + "_" + std::to_string(oper.mnc) + ".png";

            operatorItem->setText(0, QString::fromStdString(operatorItemText));
            operatorItem->setData(0, Qt::UserRole, QString::fromStdString(operatorItemIcon));
        }
    }

    expandAll();
}
