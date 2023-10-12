#include "RouteTableWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QSqlRecord>
#include <QLabel>

#include "common.h"

RouteTable::RouteTable(QWidget *parent)
    : QWidget{parent}
    , table_delegate(parent)
    , route_model_(parent)
{
    auto table_lay = new QVBoxLayout(this);

    route_db_ = new RouteDB(this);
    table_model_ = new QSqlTableModel(this/*, route_db_->DB()*/);
    table_model_->setTable(MAIN_TABLE);
    table_model_->select();

    table_view_ = new QTableView(this);
    table_view_->setModel(table_model_);
    table_view_->setItemDelegate(&table_delegate);
    table_view_->setColumnHidden(0,true);//id
    table_view_->setColumnHidden(4,true);//DATE

    auto button_lay = new QHBoxLayout(this);
    button_lay->setSpacing(5);

    add_route_button_ = new QPushButton("create new route", this);
    remove_route_button_ = new QPushButton("delete route", this);

    button_lay->addWidget(add_route_button_);
    button_lay->addWidget(remove_route_button_);

    table_lay->addWidget(table_view_);
    table_lay->addItem(button_lay);

    connect(remove_route_button_, SIGNAL(clicked(bool)), this, SLOT(removeRouteButtonClicked()));
    connect(add_route_button_, SIGNAL(clicked(bool)), this, SLOT(addRouteButtonClicked()));
    connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onTableViewClicked(const QModelIndex&)));
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
        emit route_model_.addRoute();
    }
}

void RouteTable::onAddRoute(const RouteInfo& info)
{

    route_db_->inserIntoTable(info);
    table_model_->select();

    route_model_.setRoute(info);
    emit route_model_.addRoute();
}

void RouteTable::onTableViewClicked(const QModelIndex &index)
{
    table_view_->selectRow(index.row());
    qDebug() << "click" << index.row();
}

void RouteTable::addRouteButtonClicked()
{
    emit openRouteDialog();
}

void RouteTable::removeRouteButtonClicked()
{
    QItemSelectionModel* selectionModel = table_view_->selectionModel();
    QModelIndexList indexes = selectionModel->selectedRows();

    if(indexes.isEmpty()){
        return; //TODO: add error handling
    }

    int route_index_to_delete = indexes.at(0).row();

    table_model_->removeRow(route_index_to_delete);
    table_model_->select();

    emit route_model_.removeRoute(route_index_to_delete);
}


