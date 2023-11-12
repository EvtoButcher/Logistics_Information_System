#ifndef TABLEDELEGATE_H
#define TABLEDELEGATE_H

#include <QObject>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QItemDelegate>

class QPainter;

class TableDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    TableDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    QItemDelegate def_delegate;
};

#endif // TABLEDELEGATE_H
