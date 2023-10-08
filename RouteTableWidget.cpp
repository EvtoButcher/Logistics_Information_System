#include <QHBoxLayout>
#include <QDebug>

#include "RouteTableWidget.h"

RouteTable::RouteTable(QWidget *parent)
    : QWidget{parent}
    , route_model_(parent)
    , route_db_()
{       

    auto table_ray = new QHBoxLayout(this);

    table_model_ = new QSqlTableModel(this, route_db_.DB());
    table_model_->setTable("Routes");
    table_model_->select();

    table_view_ = new QTableView(this);
    table_view_->setModel(table_model_);

    table_ray->addWidget(table_view_);
    //table_ray->addWidget(route_table_);

}
