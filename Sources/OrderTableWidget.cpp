#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QSqlRecord>
#include <QModelIndex>
#include <QLabel>
#include <QSqlTableModel>
#include <QTableView>
#include <QPushButton>
#include <QFuture>
#include <QtConcurrent>
#include <QColorDialog>

#include "Headers/OrderTableWidget.h"
#include "Headers/common.h"

OrderTable::OrderTable(const ApplicationSettings& setting, QWidget *parent)
    : QWidget{parent}
    , table_delegate(parent)
    , route_model_(parent)
{
    auto table_lay = new QVBoxLayout(this);

    route_db_ = new OrderDB(setting, this);
    table_model_ = new QSqlTableModel(this, route_db_->DB());
    table_model_->setTable(MAIN_TABLE);
    table_model_->setEditStrategy(QSqlTableModel::OnFieldChange);
    table_model_->select();

    table_view_ = new QTableView(this);
    table_view_->setItemDelegate(&table_delegate);
    table_view_->setModel(table_model_);
    table_view_->resizeColumnsToContents();
    table_view_->setColumnWidth(1, 1);
    table_view_->setColumnHidden(0,true);//id
    table_view_->setColumnHidden(2,true);//StartPos
    table_view_->setColumnHidden(3,true);//EndPos

    auto button_lay = new QHBoxLayout(this);
    button_lay->setSpacing(5);

    add_order_button_ = new QPushButton("Create an order", this);
    //add_order_button_->setEnabled(false);
    remove_route_button_ = new QPushButton("delete an order", this);
    remove_route_button_->setEnabled(false);

    button_lay->addWidget(add_order_button_);
    button_lay->addWidget(remove_route_button_);

    table_lay->addWidget(table_view_);
    table_lay->addItem(button_lay);

    connect(&route_model_, &RouteModel::selectRouteOnMap, this, &OrderTable::routeOnMapSelected);
    connect(&route_model_, &RouteModel::unselectRouteOnMap, this, &OrderTable::routeOnMapUnselected);
    connect(table_view_->selectionModel(), &QItemSelectionModel::selectionChanged, this, &OrderTable::rowSelected);
    connect(remove_route_button_, &QAbstractButton::clicked, this, &OrderTable::removeOrderButtonClicked);
    connect(add_order_button_, &QAbstractButton::clicked, this, &OrderTable::addOrderButtonClicked);
    connect(table_view_, &QAbstractItemView::doubleClicked, this, &OrderTable::onTableViewClicked);
}

void OrderTable::loadSettings(const ApplicationSettings &setting)
{

}

RouteModel &OrderTable::getRouteModel()
{
    return route_model_;
}

void OrderTable::restoreRoutOnMap()//TODO: lock thread
{
    if(!table_model_->rowCount()) {
        return;
    }

    for (int row = 0; row < table_model_->rowCount(); ++row) {

        auto code = table_model_->data(table_model_->index(row, 7)).toString();
        RouteInfo info(code,//code
                       common::splitCoordinates(table_model_->data(table_model_->index(row, 2)).toString()),//start position
                       common::splitCoordinates(table_model_->data(table_model_->index(row, 3)).toString()),//end position
                       route_db_->selectPath(code), //cache
                       table_model_->data(table_model_->index(row, 1)).toString());//color

        route_model_.setRoute(info);

        emit route_model_.restorRoute();

        while(route_model_.checkPathCacheStatus() != UploadStatus::Colpleted); //waiting for the route to be loaded on the map
        QThread::msleep(10);
    }

}

void OrderTable::onAddOrder(const RouteInfo& info)
{
    route_db_->inserIntoOrderTable(info);
    table_model_->select();
    table_view_->resizeColumnsToContents();

    route_model_.setRoute(info);
    emit route_model_.addRoute();

    QFuture<void> future = QtConcurrent::run(this, &OrderTable::setPathCacheAndDistance);
}

void OrderTable::onTableViewClicked(const QModelIndex &index)
{
    table_view_->selectRow(index.row());

    if(index.column() != 1){
        return;
    }

    QItemSelectionModel* selectionModel = table_view_->selectionModel();
    QModelIndexList indexes = selectionModel->selectedRows();

    auto color_dialog = new QColorDialog(route_db_->selectColor(indexes.at(0).data(Qt::DisplayRole).toInt()) ,this);
    color_dialog->exec();

    auto color = color_dialog->currentColor().name();

    route_db_->updateColor(indexes.at(0).data(Qt::DisplayRole).toInt(), color);
    table_model_->selectRow(index.row());

    emit route_model_.colorChenged(index.row(), color);
    table_view_->clearSelection();
}

void OrderTable::orderAddWidgetIsVisible()
{
    add_order_button_->isVisible() ? add_order_button_->hide() : add_order_button_->show();
}

void OrderTable::addOrderButtonClicked()
{
    emit openOrderDialog();
}

void OrderTable::removeOrderButtonClicked()
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

    route_db_->deleteFromOrderTable(route_index_to_delete);
    table_model_->select();

    emit route_model_.removeRoute(indexes.at(0).row());

}

void OrderTable::rowSelected()
{
    QItemSelectionModel* selectionModel = table_view_->selectionModel();
    QModelIndexList indexes = selectionModel->selectedRows();

    if(!indexes.empty()) {
        remove_route_button_->setEnabled(true);
        for(int index = 0; index < indexes.size(); ++index){
            emit route_model_.selectRouteOnTable(indexes[index].row());
        }
    }
    else {
        remove_route_button_->setEnabled(false);                       //
        for(int index = 0; index < table_model_->rowCount(); ++index){ // Not the best option
            emit route_model_.unSelectRouteOnTable();                  //
        }
    }
}

void OrderTable::routeOnMapSelected(int index)
{
    table_view_->selectRow(index);
}

void OrderTable::routeOnMapUnselected()
{
    table_view_->clearSelection();
}

void OrderTable::setPathCacheAndDistance()
{
    while(route_model_.checkRouteStatus() != UploadStatus::Colpleted);
    QThread::sleep(1);

    route_db_->insrtrIntoPathTable(route_model_.getInfo().code_, route_model_.getInfo().path_cache_);
    route_db_->updateDistanceFromOrderTable(route_model_.getInfo().code_, route_model_.getInfo().path_distance_);

    table_model_->select();
}

void OrderTable::closeDbConnection()
{
    emit route_model_.removeAllRoutes();
    route_db_->closeDB();
    table_model_->select();
}

void OrderTable::importFromDB()
{
    route_db_->importDB();
    table_model_->setTable(MAIN_TABLE);
    table_model_->select();

    table_view_->setModel(table_model_);
    table_view_->setItemDelegate(&table_delegate);
    table_view_->resizeColumnsToContents();
    table_view_->setColumnWidth(1, 1);
    table_view_->setColumnHidden(0,true);//id
    table_view_->setColumnHidden(2,true);//StartPos
    table_view_->setColumnHidden(3,true);//EndPos

    QFuture<void> future = QtConcurrent::run(this, &OrderTable::restoreRoutOnMap);
}


