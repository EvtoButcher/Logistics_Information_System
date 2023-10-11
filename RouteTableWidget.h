#ifndef ROUTETABLE_H
#define ROUTETABLE_H

#include <QWidget>
#include <QSqlTableModel>
#include <QTableView>

#include "RouteModel.h"
#include "RouteDB.h"
#include "TableDelegate.h"


class RouteTable : public QWidget
{
    Q_OBJECT

    QSqlTableModel*     table_model_;
    QTableView*         table_view_;

public:
    explicit RouteTable(QWidget *parent = nullptr);

    RouteModel& getRouteModel();
    void restoreRoutOnMap();

private:
    RouteModel      route_model_;
    TableDelegate table_delegate;
    RouteDB*         route_db_;

public slots:
    void onAddRoute(const RouteInfo& info);
    void onTableViewClicked(const QModelIndex& index);
};

#endif // ROUTETABLE_H
