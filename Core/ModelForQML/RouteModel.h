#ifndef ROUTEMODEL_H
#define ROUTEMODEL_H

#include <QVariant>
#include <QObject>
#include <utility>
#include <QString>
#include <QJSValue>
#include <QVector>
#include <QGeoCoordinate>

#include "AbstractMapItemModel.h"

struct RouteInfo {
    RouteInfo() = default;

    explicit RouteInfo(const QString& name, double start_lat, double satrt_lng,
                       double end_lat, double end_lng, const QString& color = "#008000" /*green*/);

    explicit RouteInfo(const QString& name, const QGeoCoordinate start, const QGeoCoordinate end,
                       const QString& path_cache, const QString& color = "#008000" /*green*/);

    QString        code_;
    QGeoCoordinate start_route_point_;
    QGeoCoordinate end_route_point_;
    QString        route_color_;

    int path_distance_;
    QVector<QGeoCoordinate> path_cache_;
};

enum UploadRouteStatus {
    Null,
    Colpleted,
    Loading,
    Error
};

class RouteModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double       StartLat   READ  startLat)
    Q_PROPERTY(double       StartLng   READ  startLng)
    Q_PROPERTY(double       EndLat     READ  endLat)
    Q_PROPERTY(double       EndLng     READ  endLng)
    Q_PROPERTY(QString      RouteColor READ  routeColor)
    Q_PROPERTY(QVariantList RoutePath  READ  routePath CONSTANT)

public:
    explicit RouteModel(QObject* parent = nullptr);

    double       startLat()     const;
    double       startLng()     const;
    double       endLat()       const;
    double       endLng()       const;
    QString      routeColor()   const;
    QVariantList routePath()    const;

    void setRoute(const RouteInfo& new_route);

    Q_INVOKABLE void setPathCache(const QJSValue& path);
    Q_INVOKABLE void setRouteStatus(int current_status /*UploadStatus*/);
    Q_INVOKABLE void setPathCacheStatus(int current_status /*UploadStatus*/);

    const RouteInfo&  getInfo() const;
    UploadRouteStatus checkRouteStatus();
    UploadRouteStatus checkPathCacheStatus();

signals:
    void addRoute();
    void restorRoute();
    void removeRoute(int);
    void removeAllRoutes();
    void unselectRouteOnMap();
    void selectRouteOnMap(int);
    void unSelectRouteOnTable();
    void selectRouteOnTable(int);
    void colorChenged(int, QString);

private:
    RouteInfo         route_;
    UploadRouteStatus route_status_ = UploadRouteStatus::Null;
    UploadRouteStatus path_cache_status_ = UploadRouteStatus::Null;
};

#endif // ROUTEMODEL_H
