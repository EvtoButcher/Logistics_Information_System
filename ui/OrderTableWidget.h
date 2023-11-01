#ifndef ROUTETABLE_H
#define ROUTETABLE_H

#include <QWidget>
#include <QTableView>
#include <QVariant>

#include "OrderDB.h"
#include "TableDelegate.h"

class QSqlTableModel;
class QTableView;
class QPushButton;

class OrderTable : public QWidget
{
    Q_OBJECT
public:
    explicit OrderTable(const OrderDB *db, QWidget *parent = nullptr);

public slots:
    void updateTable();
    void orderAddWidgetIsVisible();
    void routeOnMapSelected(int index);
    void routeOnMapUnselected();

signals:
    void openOrderDialog();
    void removeRoute(const int, const int);
    void onSelectRouteOnTable(const int);
    void onUnselectRouteOnTable();
    void updateColor(const int, const int, const QString&);

private slots:
    void addOrderButtonClicked();
    void removeOrderButtonClicked();
    void onTableViewColorClicked(const QModelIndex& index);
    void rowSelected();

private:
    void setPathCacheAndDistance();

private:
    QSqlTableModel*  table_model_;
    QTableView*      table_view_;
    TableDelegate    table_delegate;

    QPushButton*     add_order_button_;
    QPushButton*     remove_route_button_;
};

#endif // ROUTETABLE_H
