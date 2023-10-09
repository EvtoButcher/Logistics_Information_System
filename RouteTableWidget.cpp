#include <QHBoxLayout>
#include <QDebug>
#include <QSqlRecord>
#include <QVector>

#include "RouteTableWidget.h"
#include "common.h"

RouteTable::RouteTable(QWidget *parent)
    : QWidget{parent}
    , route_model_(parent)
{       
    auto table_ray = new QHBoxLayout(this);

    route_db_ = new RouteDB(this);
    table_model_ = new QSqlTableModel(this/*, route_db_->DB()*/);
    table_model_->setTable(MAIN_TABLE);
    table_model_->select();

    table_view_ = new QTableView(this);
    table_view_->setModel(table_model_);
    table_view_->setColumnHidden(0,true);//id
    table_view_->setColumnHidden(4,true);//DATE
//    table_view_->resizeColumnsToContents();
//    table_view_->resizeColumnsToContents();
//    table_view_->setEditTriggers(QAbstractItemView::NoEditTriggers);

    table_ray->addWidget(table_view_);
}

RouteModel &RouteTable::getRouteModel()
{
    return route_model_;
}


void RouteTable::restoreRoutOnMap()
{
    if(!table_model_->rowCount()){
        return;
    }
    //qDebug() << table_model_->record(1);

    QVector<RouteInfo>tmp;
    tmp.reserve(table_model_->rowCount());
    for (int row = 0; row < table_model_->rowCount(); ++row) {
        RouteInfo info;
        info.name_ = table_model_->data(table_model_->index(row, 1)).toString(); // Получаем значение поля
        info.start_route_point_ = splitCoordinates(table_model_->data(table_model_->index(row, 2)).toString());
        info.end_route_point_ = splitCoordinates(table_model_->data(table_model_->index(row, 3)).toString());
        info.route_color_ = table_model_->data(table_model_->index(row, 5)).toString();

        tmp.append(info);

//        route_model_.setRoute(info);
//        emit route_model_.add_route_();

    }
    for (const auto& info: tmp) {
        route_model_.setRoute(info);
        emit route_model_.add_route_();
    }
}


void RouteTable::onAddRoute(const RouteInfo& info)
{
    route_db_->inserIntoTable(info);
    table_model_->select();

    route_model_.setRoute(info);
    emit route_model_.add_route_();
}


