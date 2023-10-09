#include <QHBoxLayout>
#include <QDebug>

#include "RouteTableWidget.h"

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
    table_view_->setColumnHidden(0,true);
//    table_view_->resizeColumnsToContents();
//    table_view_->resizeColumnsToContents();
//    table_view_->setEditTriggers(QAbstractItemView::NoEditTriggers);

    table_ray->addWidget(table_view_);
}

RouteModel &RouteTable::GetRouteModel()
{
    return route_model_;
}


void RouteTable::onAddRoute(const RouteInfo& info)
{
    route_db_->inserIntoTable(info);
    table_model_->select();

    route_model_.setRoute(info);
    emit route_model_.add_route_();
}


