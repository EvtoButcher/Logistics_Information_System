#include <QPainter>

#include "Headers/TableDelegate.h"

TableDelegate::TableDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
    , def_delegate(parent)
{
}

void TableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 1){
        QString color = index.model()->data(index).toString();
        if (!color.isEmpty()) {
            painter->fillRect(option.rect, QColor(color));
        }
        else {
             def_delegate.paint(painter, option, index);
        }
    }
    else {
       def_delegate.paint(painter, option, index);
   }
};

