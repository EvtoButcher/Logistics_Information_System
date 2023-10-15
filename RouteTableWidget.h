#ifndef ROUTETABLE_H
#define ROUTETABLE_H

#include <QWidget>
#include <QTableView>

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
    explicit RouteTable(QWidget *parent = nullptr);

    RouteModel& getRouteModel();
    void restoreRoutOnMap();

    void closeDbConnection();
    void importFromDB();

public slots:
    void onAddRoute(const RouteInfo& info);
    void onTableViewClicked(const QModelIndex& index);

signals:
    void openRouteDialog();

private slots:
    void addRouteButtonClicked();
    void removeRouteButtonClicked();

    void rowSelected();
    void routeOnMapSelected(int index);

private:
    QSqlTableModel*  table_model_;
    QTableView*      table_view_;
    TableDelegate    table_delegate;
    RouteDB*         route_db_;
    RouteModel       route_model_;

    QPushButton*     add_route_button_;
    QPushButton*     remove_route_button_;
};


//void restoreRoutOnMap(RouteTable* table);

#endif // ROUTETABLE_H
