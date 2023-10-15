#include "RouteTableWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QSqlRecord>
#include <QModelIndex>
#include <QLabel>
#include <QSqlTableModel>
#include <QTableView>
#include <QPushButton>
#include <QVector>

#include "common.h"

RouteTable::RouteTable(QWidget *parent)
    : QWidget{parent}
    , table_delegate(parent)
    , route_model_(parent)
{
    auto table_lay = new QVBoxLayout(this);

    route_db_ = new RouteDB(this);
    table_model_ = new QSqlTableModel(this, route_db_->DB());
    table_model_->setTable(MAIN_TABLE);
    table_model_->setEditStrategy(QSqlTableModel::OnFieldChange);
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

    connect(&route_model_, &RouteModel::selectRouteOnMap, this, &RouteTable::routeOnMapSelected);
    connect(table_view_->selectionModel(), &QItemSelectionModel::selectionChanged, this, &RouteTable::rowSelected);
    connect(remove_route_button_, &QAbstractButton::clicked, this, &RouteTable::removeRouteButtonClicked);
    connect(add_route_button_, &QAbstractButton::clicked, this, &RouteTable::addRouteButtonClicked);
    connect(table_view_, &QAbstractItemView::doubleClicked, this, &RouteTable::onTableViewClicked);
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

        RouteInfo info(table_model_->data(table_model_->index(row, 1)).toString(),//name
                       splitCoordinates(table_model_->data(table_model_->index(row, 2)).toString()),//start position
                       splitCoordinates(table_model_->data(table_model_->index(row, 3)).toString()),//end position
                       table_model_->data(table_model_->index(row, 5)).toString());//color

        route_model_.setRoute(info);

        emit route_model_.addRoute();

        while(route_model_.checkStatus() != UploadStatus::Colpleted); //waiting for the route to be loaded on the map
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

//    for(const auto& index : indexes){
//        int route_index_to_delete = index.data(Qt::DisplayRole).toInt();
//        route_db_->deleteFromTable(route_index_to_delete);
//        emit route_model_.removeRoute(index.row());
//    }

    auto route_index_to_delete = indexes.at(0).data(Qt::DisplayRole).toInt();

    route_db_->deleteFromTable(route_index_to_delete);
    table_model_->select();

    emit route_model_.removeRoute(indexes.at(0).row());

}

void RouteTable::rowSelected()
{
    QItemSelectionModel* selectionModel = table_view_->selectionModel();
    QModelIndexList indexes = selectionModel->selectedRows();

    if(!indexes.empty()){
        emit route_model_.selectRouteOnTable(indexes[0].row());
    }
    else{
        route_model_.unSelectRouteOnTable();
    }
}

void RouteTable::routeOnMapSelected(int index)
{
    table_view_->selectRow(index);
}


void RouteTable::closeDbConnection()
{
    emit route_model_.removeAllRoutes();
    route_db_->closeDB();
    table_model_->select();
    //table_view_->update();
}

void RouteTable::importFromDB()
{
    route_db_->importDB();
}


