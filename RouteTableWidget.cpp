#include "RouteTableWidget.h"

#include <QVBoxLayout>
#include <QDebug>
#include <QSqlRecord>
#include <QLabel>

#include "common.h"

RouteTable::RouteTable(QWidget *parent)
    : QWidget{parent}
    , route_model_(parent)
    , table_delegate(parent)
{       
    auto table_ray = new QVBoxLayout(this);

    route_db_ = new RouteDB(this);
    table_model_ = new QSqlTableModel(this/*, route_db_->DB()*/);
    table_model_->setTable(MAIN_TABLE);
    table_model_->select();

    auto table_label = new QLabel("Route table", this);
    table_label->setFont(QFont("Ubuntu", 11, QFont::Bold));

    table_view_ = new QTableView(this);
    table_view_->setModel(table_model_);
    table_view_->setItemDelegate(&table_delegate);

    table_view_->setColumnHidden(0,true);//id
    table_view_->setColumnHidden(4,true);//DATE

    table_ray->addWidget(table_label);
    table_ray->addWidget(table_view_);

    connect(table_view_, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onTableViewClicked(const QModelIndex&)));
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

    for (int row = 0; row < table_model_->rowCount(); ++row) {

        RouteInfo info(table_model_->data(table_model_->index(row, 1)).toString(),
                       splitCoordinates(table_model_->data(table_model_->index(row, 2)).toString()),
                       splitCoordinates(table_model_->data(table_model_->index(row, 3)).toString()),
                       table_model_->data(table_model_->index(row, 5)).toString());


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

void RouteTable::onTableViewClicked(const QModelIndex &index)
{
    table_view_->selectRow(index.row());
    qDebug() << "click" << index.row();
}


