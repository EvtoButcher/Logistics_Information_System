#ifndef ROUTEMODEL_H
#define ROUTEMODEL_H

#include <QObject>
#include <utility>
#include <QString>

using Position = std::pair<double, double>;

struct RouteInfo{
    RouteInfo() = default;
    RouteInfo(QString name, double start_lat, double satrt_lng,
              double end_lat, double end_lng,
              QString color = "#008000" /*green*/);

    RouteInfo(QString name, Position start, Position end,
              QString color = "#008000" /*green*/);

    QString name_;
    Position start_route_point_;
    Position end_route_point_;
    QString route_color_;
};

class RouteModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double StartLat READ startLat)
    Q_PROPERTY(double StartLng READ startLng)
    Q_PROPERTY(double EndLat READ endLat)
    Q_PROPERTY(double EndLng READ endLng)
    Q_PROPERTY(QString RouteColor READ routeColor)

public:
    explicit RouteModel(QObject *parent = nullptr);

    double startLat();
    double startLng();
    double endLat();
    double endLng();

    QString routeColor();

    void setRoute(const RouteInfo &newRoute);

signals:
    void addRoute();
    void removeRoute(int index);

private:
    RouteInfo route;
};

#endif // ROUTEMODEL_H
