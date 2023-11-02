#ifndef APPENGINE_H
#define APPENGINE_H
#include <QObject>
#include <QQmlContext>
#include <QVector>

#include "RouteModel.h"
#include "WarehouseModel.h"
#include "DestinationModel.h"

class OrderDB;
class ApplicationSettings;


class MapItemEngine : public QObject
{
    Q_OBJECT
public:
    MapItemEngine(QObject *parent = nullptr);

    RouteModel&         getRouteModel();
    WarehouseModel&     getWarehouseModel();
    DestinationModel&   getDestinationModel();
    const OrderDB*      getDB();

    void OpenDb(const QString& name);
    void restoreMap();

signals:
    void distanceUpdated();

public slots:
    void addRoute(const RouteInfo& info);
    void removeRoute(const int db_index, const int map_index);
    void routeColorUpdate(const int db_index, const int map_index, const QString& color);

    void addWarehouse(const uint64_t code, const QGeoCoordinate position);
    void addDestination(const uint64_t code, const QGeoCoordinate position);

private:
    void setPathCacheAndDistance();

private:
    OrderDB*          route_db_;
    RouteModel        route_model_;
    WarehouseModel    warehouse_model_;
    DestinationModel  destination_modal_;
};


#endif // APPENGINE_H
