#include "MapItemEngine.h"

#include <QSqlTableModel>
#include <QFuture>
#include <QtConcurrent>

#include "common.h"
#include "OrderDB.h"
#include "ApplicationSettings.h"

MapItemEngine::MapItemEngine(QObject* parent)
    : QObject(parent)
    , route_model_(parent)
    , warehouse_model_(parent)
{
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

    if(table_model.rowCount()) {

        for (int row = 0; row < table_model.rowCount(); ++row) {

            auto code = table_model.data(table_model.index(row, 7)).toString();                              // code
            RouteInfo info(code,                                                                             //
                           common::splitCoordinates(table_model.data(table_model.index(row, 2)).toString()), // start position
                           common::splitCoordinates(table_model.data(table_model.index(row, 3)).toString()), // end position
                           route_db_->selectPath(code),                                                      // path cache from DB
                           table_model.data(table_model.index(row, 1)).toString());                          // color

            route_model_.setRoute(info);

            emit route_model_.restorRoute();

            while(route_model_.checkPathCacheStatus() != UploadRouteStatus::Colpleted); //waiting for the route to be loaded on the map
            QThread::msleep(10);

            traffic_->getTraffic().push_back(TrafficInfo{info.path_cache_, info.route_color_});
        }
    }

    table_model.setTable(WAREHOUSE_TABLE);
    table_model.select();

    if(table_model.rowCount()) {

        for (int row = 0; row < table_model.rowCount(); ++row) {

            map_Item_Info::WarehouseInfo info (table_model.data(table_model.index(row, 1)).toInt(),                               // code
                                common::splitCoordinates(table_model.data(table_model.index(row, 2)).toString())); // position

            warehouse_model_.setWarehouse(info);

            emit warehouse_model_.restorWarehouse();

            while(warehouse_model_.checkWarehouseStatus() != UploadWarehouseStatus::Colpleted);
            QThread::msleep(10);
        }
    }

    table_model.setTable(DESTINATION_TABLE);
    table_model.select();

    if(table_model.rowCount()) {

        for (int row = 0; row < table_model.rowCount(); ++row) {

            map_Item_Info::DestinationInfo info (table_model.data(table_model.index(row, 1)).toInt(),                             // code
                                common::splitCoordinates(table_model.data(table_model.index(row, 2)).toString())); // position

            destination_modal_.setDestination(info);

            emit destination_modal_.addDestination();

            while(destination_modal_.checkDestinationStatus() != UploadDestinationStatus::Colpleted);
            QThread::msleep(10);
        }
    }

    if(!traffic_->trafficIsEmpty()) {
        traffic_->start();
    }
}

WarehouseModel& MapItemEngine::getWarehouseModel()
{
    return warehouse_model_;
}

DestinationModel& MapItemEngine::getDestinationModel()
{
    return destination_modal_;
}

AbstractTrifficModel* MapItemEngine::getTrafficModel()
{
    return traffic_;
}

const OrderDB* MapItemEngine::getDB()
{
    return route_db_;
}

void MapItemEngine::OpenDb(const QString& name)
{
    route_db_ = new OrderDB(name, this);
}

void MapItemEngine::addRoute(const RouteInfo& info)
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

void MapItemEngine::addDestination(const uint64_t code, const QGeoCoordinate position)
{
    route_db_->insertIntoDestinationTable(code, position);
    destination_modal_.setDestination({code, position});

    emit destination_modal_.addDestination();
}

void MapItemEngine::addTraffic(const uint64_t code, const QGeoCoordinate position)
{
    //route_db_->insertIntoTrafficTable(code, position);
    //destination_modal_.setDestination({code, position});
    //emit destination_modal_.addDestination();
}

void MapItemEngine::advanceNextTrafficPoint(const QGeoCoordinate position)
{

}

void MapItemEngine::setPathCacheAndDistance()
{
    while(route_model_.checkRouteStatus() != UploadRouteStatus::Colpleted);
    QThread::sleep(1);

    route_db_->insrtrIntoPathTable(route_model_.getInfo().code_, route_model_.getInfo().path_cache_);
    route_db_->updateDistanceFromOrderTable(route_model_.getInfo().code_, route_model_.getInfo().path_distance_);

    emit distanceUpdated();
}

