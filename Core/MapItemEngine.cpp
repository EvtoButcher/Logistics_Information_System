#include "MapItemEngine.h"

#include <QSqlTableModel>

#include "common.h"
#include "OrderDB.h"
#include "ApplicationSettings.h"
#include "Company.h"

MapItemEngine::MapItemEngine(QObject* parent)
    : QObject(parent)
    , route_model_()
    , warehouse_model_()
{
    connect(&route_model_, &RouteModel::pathCacheReady,
            this, &MapItemEngine::onPathCacheReady);
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

            traffic_->addTraffic(TrafficInfo{info.path_cache_, info.route_color_});
        }
    }

    table_model.setTable(WAREHOUSE_TABLE);
    table_model.select();

    if(table_model.rowCount()) {

        for (int row = 0; row < table_model.rowCount(); ++row) {

            map_Item_Info::WarehouseInfo info (table_model.data(table_model.index(row, 1)).toULongLong(),                          // code
                                common::splitCoordinates(table_model.data(table_model.index(row, 2)).toString())); // position

            warehouse_model_.setWarehouse(info);

            emit warehouse_model_.restorWarehouse();
        }
    }

    table_model.setTable(DESTINATION_TABLE);
    table_model.select();

    if(table_model.rowCount()) {

        for (int row = 0; row < table_model.rowCount(); ++row) {

            map_Item_Info::DestinationInfo info (table_model.data(table_model.index(row, 1)).toULongLong(),                        // code
                                common::splitCoordinates(table_model.data(table_model.index(row, 2)).toString())); // position

            destination_modal_.setDestination(info);

            emit destination_modal_.addDestination();
        }
    }

    // Truck simulation is started explicitly from the DevTools menu (PR 5).
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
    route_db_->insertIntoOrderTable(info);
    route_model_.setRoute(info);
    emit route_model_.addRoute();
}

void MapItemEngine::removeRoute(const int db_index, const int map_index)
{
    route_db_->deleteFromOrderTable(db_index);

    emit route_model_.removeRoute(map_index);
}

void MapItemEngine::routeColorUpdate(const int db_index, const int map_index, const QString& color)
{
    route_db_->updateColor(db_index, color);

    emit route_model_.colorChenged(map_index, color);
}

void MapItemEngine::addWarehouse(Warehouse* warehouse)
{
    if(!warehouse) {
        return;
    }

    const uint64_t code = warehouse->getCode();
    const QGeoCoordinate position = warehouse->getPosition();

    route_db_->insertIntoWarehouseTable(code, position);

    for(const auto& [cargoCode, cargo] : warehouse->getCargo()) {
        if(cargo) {
            route_db_->insertCargo(QString::number(code), cargoCode, cargo->weight_, cargo->volume_);
        }
    }

    warehouse_model_.setWarehouse({code, position});

    emit warehouse_model_.addWarehouse();
}

void MapItemEngine::addDestination(const uint64_t code, const QGeoCoordinate position)
{
    route_db_->insertIntoDestinationTable(code, position);
    destination_modal_.setDestination({code, position});

    emit destination_modal_.addDestination();
}

void MapItemEngine::removeWarehouse(const uint64_t code, const int index)
{
    route_db_->deleteWarehouse(code);
    emit warehouse_model_.removeWarehouse(index);
}

void MapItemEngine::removeDestination(const uint64_t code, const int index)
{
    route_db_->deleteDestination(code);
    emit destination_modal_.removeDestination(index);
}

void MapItemEngine::onPathCacheReady()
{
    if(!route_db_ || !route_db_->isOpen()) {
        return;
    }

    const RouteInfo& info = route_model_.getInfo();
    route_db_->insertIntoPathTable(info.code_, info.path_cache_);
    route_db_->updateDistanceFromOrderTable(info.code_, info.path_distance_);

    if(traffic_) {
        traffic_->addTraffic(TrafficInfo{info.path_cache_, info.route_color_});
    }

    emit distanceUpdated();
}

