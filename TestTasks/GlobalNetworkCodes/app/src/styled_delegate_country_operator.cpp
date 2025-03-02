#include "app/styled_delegate_country_operator.h"

#include <QDebug>
#include <QIcon>
#include <QString>
#include <QTreeWidgetItem>

void gnc::StyledDelegateCountryOperator::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QString iconPath = index.data(Qt::UserRole).toString();
    QPixmap pixmap(iconPath);

    QSize iconSize{ 16, 16 };

    if (!iconPath.isEmpty()) {
        QIcon icon(iconPath);

        if (icon.isNull()) {
            qDebug() << "Icon not found at path:" << iconPath;
        }
        else {
            QRect iconRect = option.rect;
            iconRect.setWidth(iconSize.width());
            icon.paint(painter, iconRect, Qt::AlignLeft | Qt::AlignVCenter);
        }
    }

    QTreeWidgetItem* item = static_cast<QTreeWidgetItem*>(index.internalPointer());
    if (!item) {
        return QStyledItemDelegate::paint(painter, option, index);
    }

    QString text = index.data(Qt::DisplayRole).toString();
    QRect textRect = option.rect;
    textRect.setLeft(option.rect.left() + iconSize.width() * 1.4);

    QFont font = option.font;

    if (item->parent() == nullptr) {
        font.setBold(true);
    }

    painter->setFont(font);
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);
}
