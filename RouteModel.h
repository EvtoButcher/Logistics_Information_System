#ifndef ROUTEMODEL_H
#define ROUTEMODEL_H

#include <QObject>
#include <utility>
#include <QString>

using Position = std::pair<double, double>;

struct RouteInfo{
    RouteInfo() = default;
    RouteInfo(QString name, double s_lat, double s_lng,
              double e_lat, double e_lng,
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
    Q_PROPERTY(double StartLat READ StartLat)
    Q_PROPERTY(double StartLng READ StartLng)
    Q_PROPERTY(double EndLat READ EndLat)
    Q_PROPERTY(double EndLng READ EndLng)
    Q_PROPERTY(QString RouteColor READ RouteColor)

public:
    explicit RouteModel(QObject *parent = nullptr);

    double StartLat();
    double StartLng();
    double EndLat();
    double EndLng();

    QString RouteColor();

    void setRoute(const RouteInfo &newRoute);

private:
    RouteInfo route;

signals:
    void add_route_();

};

#endif // ROUTEMODEL_H
