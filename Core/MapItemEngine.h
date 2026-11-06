#ifndef APPENGINE_H
#define APPENGINE_H
#include <QObject>
#include <QQmlContext>
#include <QVector>

#include "RouteModel.h"
#include "WarehouseModel.h"
#include "DestinationModel.h"
#include "AbstractTrafficModel.h"

class OrderDB;
class ApplicationSettings;
class Warehouse;

class MapItemEngine : public QObject
{
    Q_OBJECT

public:
    explicit MapItemEngine(QObject* parent = nullptr);

    RouteModel&             getRouteModel();
    WarehouseModel&         getWarehouseModel();
    DestinationModel&       getDestinationModel();
    AbstractTrifficModel*   getTrafficModel();
    const OrderDB*          getDB();

    void OpenDb(const QString& name);
    void restoreMap();

public:
    AbstractTrifficModel* traffic_;

signals:
    void distanceUpdated();

public slots:
    void addRoute(const RouteInfo& info);
    void removeRoute(const int db_index, const int map_index);
    void routeColorUpdate(const int db_index, const int map_index, const QString& color);

    void addWarehouse(Warehouse* warehouse);
    void addDestination(const uint64_t code, const QGeoCoordinate position);
    void removeWarehouse(const uint64_t code, const int index);
    void removeDestination(const uint64_t code, const int index);

private slots:
    void onPathCacheReady();

private:
    OrderDB*              route_db_ = nullptr;
    RouteModel            route_model_;
    WarehouseModel        warehouse_model_;
    DestinationModel      destination_modal_;
};


#endif // APPENGINE_H
