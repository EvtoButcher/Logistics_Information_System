#include <QSqlTableModel>
#include <QFuture>
#include <QtConcurrent>

#include "Headers/common.h"
#include "Headers/MapItemEngine.h"
#include "Headers/OrderDB.h"
#include "Headers/ApplicationSettings.h"

MapItemEngine::MapItemEngine(const ApplicationSettings& setting, QObject *parent)
    : QObject{parent}
    , route_model_(parent)
    , warehouse_model_(parent)
{
    route_db_ = new OrderDB(setting, this);
}

RouteModel& MapItemEngine::getRouteModel()
{
    return route_model_;
}

void MapItemEngine::restoreMap()
{
    QSqlTableModel table_model(this, route_db_->DB());
    table_model.setTable(MAIN_TABLE);
    table_model.select();

    if(!table_model.rowCount()) {
        return;
    }

    for (int row = 0; row < table_model.rowCount(); ++row) {

        auto code = table_model.data(table_model.index(row, 7)).toString();
        RouteInfo info(code,//code
                       common::splitCoordinates(table_model.data(table_model.index(row, 2)).toString()),//start position
                       common::splitCoordinates(table_model.data(table_model.index(row, 3)).toString()),//end position
                       route_db_->selectPath(code), //cache
                       table_model.data(table_model.index(row, 1)).toString());//color

        route_model_.setRoute(info);

        emit route_model_.restorRoute();

        while(route_model_.checkPathCacheStatus() != UploadStatus::Colpleted); //waiting for the route to be loaded on the map
        QThread::msleep(10);
    }

    table_model.setTable(WAREHOUSE_TABLE);

    if(!table_model.rowCount()) {
        return;
    }

    for (int row = 0; row < table_model.rowCount(); ++row) {

        WarehouseInfo info (table_model.data(table_model.index(row, 1)).toInt(),
                            common::splitCoordinates(table_model.data(table_model.index(row, 1)).toString()));

        warehouse_model_.setWarehouse(info);

        emit warehouse_model_.restorWarehouse();

        //while(route_model_.checkPathCacheStatus() != UploadStatus::Colpleted); //waiting for the route to be loaded on the map
        QThread::msleep(10);
    }
}

WarehouseModel &MapItemEngine::getWarehouseModel()
{
    return warehouse_model_;
}

const OrderDB *MapItemEngine::getDB()
{
    return route_db_;
}

void MapItemEngine::addRoute(const RouteInfo &info)
{
    route_db_->inserIntoOrderTable(info);
    route_model_.setRoute(info);
    emit route_model_.addRoute();

    QFuture<void> future = QtConcurrent::run(this, &MapItemEngine::setPathCacheAndDistance);
}

void MapItemEngine::removeRoute(const int db_index, const int map_index)
{
    route_db_->deleteFromOrderTable(db_index);

    emit route_model_.removeRoute(map_index);
}

void MapItemEngine::routeColorUpdate(const int db_index, const int map_index, const QString &color)
{
    route_db_->updateColor(db_index, color);
    emit route_model_.colorChenged(map_index, color);
}

void MapItemEngine::addWarehouse(const uint64_t code, const QGeoCoordinate position)
{
    route_db_->insertIntoWarehouseTable(code, position);
    warehouse_model_.setWarehouse({code, position});
    emit warehouse_model_.addWarehouse();
}

void MapItemEngine::setPathCacheAndDistance()
{
    while(route_model_.checkRouteStatus() != UploadStatus::Colpleted);
    QThread::sleep(1);

    route_db_->insrtrIntoPathTable(route_model_.getInfo().code_, route_model_.getInfo().path_cache_);
    route_db_->updateDistanceFromOrderTable(route_model_.getInfo().code_, route_model_.getInfo().path_distance_);

    emit distanceUpdated();
}

//void OrderTable::closeDbConnection()
//{
//    emit map_item_engine_.getRouteModel().removeAllRoutes();
//    route_db_->closeDB();
//    table_model_->select();
//}

//void OrderTable::importFromDB()
//{
//    route_db_->importDB();
//    table_model_->setTable(MAIN_TABLE);
//    table_model_->select();

//    table_view_->setModel(table_model_);
//    table_view_->setItemDelegate(&table_delegate);
//    table_view_->resizeColumnsToContents();
//    table_view_->setColumnWidth(1, 1);
//    table_view_->setColumnHidden(0,true);//id
//    table_view_->setColumnHidden(2,true);//StartPos
//    table_view_->setColumnHidden(3,true);//EndPos

//    QFuture<void> future = QtConcurrent::run(this, &OrderTable::restoreRoutOnMap);
//}