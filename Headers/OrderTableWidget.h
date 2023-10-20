#ifndef ROUTETABLE_H
#define ROUTETABLE_H

#include <QWidget>
#include <QTableView>
#include <QVariant>

#include "RouteModel.h"
#include "OrderDB.h"
#include "TableDelegate.h"

class QSqlTableModel;
class QTableView;
class QPushButton;

class OrderTable : public QWidget
{
    Q_OBJECT
public:
    explicit OrderTable(QWidget *parent = nullptr);

    RouteModel& getRouteModel();
    void restoreRoutOnMap();

    void closeDbConnection();
    void importFromDB();

public slots:
    void onAddOrder(const RouteInfo& info);
    void orderAddWidgetIsVisible();

signals:
    void openOrderDialog();

private slots:
    void addOrderButtonClicked();
    void removeOrderButtonClicked();
    void onTableViewClicked(const QModelIndex& index);

    void rowSelected();
    void routeOnMapSelected(int index);
    void routeOnMapUnselected();

private:
    void setPathCacheAndDistance();

private:
    QSqlTableModel*  table_model_;
    QTableView*      table_view_;
    TableDelegate    table_delegate;
    OrderDB*         route_db_;
    RouteModel       route_model_;

    QPushButton*     add_order_button_;
    QPushButton*     remove_route_button_;
};

#endif // ROUTETABLE_H
