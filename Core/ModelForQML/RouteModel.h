#ifndef ROUTEMODEL_H
#define ROUTEMODEL_H

#include <QVariant>
#include <QObject>
#include <utility>
#include <QString>
#include <QJSValue>
#include <QVector>
#include <QGeoCoordinate>


struct RouteInfo{
    RouteInfo() = default;
    RouteInfo(QString name, double start_lat, double satrt_lng,
              double end_lat, double end_lng,
              QString color = "#008000" /*green*/);

    RouteInfo(const QString name, const QGeoCoordinate start, const QGeoCoordinate end,
              const QString& path_cache, QString color = "#008000" /*green*/);

    QString        code_;
    QGeoCoordinate start_route_point_;
    QGeoCoordinate end_route_point_;
    QString        route_color_;

    int path_distance_;
    QVector<QGeoCoordinate> path_cache_;
};

enum class UploadRouteStatus{
    Null,
    Colpleted,
    Loading,
    Error
};

class RouteModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double  StartLat   READ startLat)
    Q_PROPERTY(double  StartLng   READ startLng)
    Q_PROPERTY(double  EndLat     READ endLat)
    Q_PROPERTY(double  EndLng     READ endLng)
    Q_PROPERTY(QString RouteColor READ routeColor)
    Q_PROPERTY(QVariantList RoutePath READ routePath CONSTANT)

public:
    explicit RouteModel(QObject *parent = nullptr);

    double       startLat();
    double       startLng();
    double       endLat();
    double       endLng();
    QString      routeColor();
    QVariantList routePath();

    void setRoute(const RouteInfo &new_route);

    Q_INVOKABLE void setPathCache(QJSValue path);
    Q_INVOKABLE void setRouteStatus(int current_status /*UploadStatus*/);
    Q_INVOKABLE void setPathCacheStatus(int current_status /*UploadStatus*/);

    UploadRouteStatus checkPathCacheStatus();
    UploadRouteStatus checkRouteStatus();
    const RouteInfo&  getInfo();

signals:
    void restorRoute();
    void addRoute();
    void removeRoute(int);
    void removeAllRoutes();
    void selectRouteOnTable(int);
    void unSelectRouteOnTable();
    void selectRouteOnMap(int);
    void unselectRouteOnMap();
    void colorChenged(int, QString);

private:
    RouteInfo         route_;
    UploadRouteStatus route_status_ = UploadRouteStatus::Null;
    UploadRouteStatus path_cache_status_ = UploadRouteStatus::Null;
};

#endif // ROUTEMODEL_H
