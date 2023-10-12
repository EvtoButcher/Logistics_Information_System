#ifndef ROUTETABLE_H
#define ROUTETABLE_H

#include <QWidget>
#include <QSqlTableModel>
#include <QTableView>
#include <QPushButton>

#include "RouteModel.h"
#include "RouteDB.h"
#include "TableDelegate.h"

class RouteTable : public QWidget
{
    Q_OBJECT
public:
    explicit RouteTable(QWidget *parent = nullptr);

    RouteModel& getRouteModel();
    void restoreRoutOnMap();

public slots:
    void onAddRoute(const RouteInfo& info);
    void onTableViewClicked(const QModelIndex& index);

private:
    QSqlTableModel*     table_model_;
    QTableView*         table_view_;
    TableDelegate table_delegate;
    RouteDB*         route_db_;
    RouteModel      route_model_;

    QPushButton* add_route_button_;
    QPushButton* remove_route_button_;
};

#endif // ROUTETABLE_H
