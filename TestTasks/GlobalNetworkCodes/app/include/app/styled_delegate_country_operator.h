#pragma once

#include <QStyledItemDelegate>
#include <QPainter>
#include <QIcon>
#include <QPixmap>

namespace gnc
{
    enum ItemTypeCategory {
        Category = 0,
        Subcategory = 1
    };

    class StyledDelegateCountryOperator : public QStyledItemDelegate
    {
    public:
        StyledDelegateCountryOperator(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}

        void paint(QPainter* painter, const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;
    };
}
