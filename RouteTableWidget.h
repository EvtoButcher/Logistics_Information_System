#ifndef ROUTETABLE_H
#define ROUTETABLE_H

#include <QWidget>
#include <QSqlTableModel>
#include <QTableView>

#include "RouteModel.h"
#include "RouteDB.h"

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
    RouteDB*         route_db_;

public slots:
    void onAddRoute(const RouteInfo& info);
};

#endif // ROUTETABLE_H
