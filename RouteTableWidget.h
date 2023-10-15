#ifndef ROUTETABLE_H
#define ROUTETABLE_H

#include <QWidget>

#include "RouteModel.h"
#include "RouteDB.h"
#include "TableDelegate.h"

class QSqlTableModel;
class QTableView;
class QPushButton;

class RouteTable : public QWidget
{
    Q_OBJECT
public:
    friend void restoreRoutOnMap(RouteTable* table) ;

    explicit RouteTable(QWidget *parent = nullptr);

    RouteModel& getRouteModel();
    //void restoreRoutOnMap();

    void closeDbConnection();
    bool status = true;
public slots:
    void onAddRoute(const RouteInfo& info);
    void onTableViewClicked(const QModelIndex& index);

signals:
    void openRouteDialog();

private slots:
    void addRouteButtonClicked();
    void removeRouteButtonClicked();

private:
    QSqlTableModel*  table_model_;
    QTableView*      table_view_;
    TableDelegate    table_delegate;
    RouteDB*         route_db_;
    RouteModel       route_model_;

    QPushButton*     add_route_button_;
    QPushButton*     remove_route_button_;
};


void restoreRoutOnMap(RouteTable* table);

#endif // ROUTETABLE_H
