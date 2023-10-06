#ifndef ROUTEMODEL_H
#define ROUTEMODEL_H

#include <QObject>
#include <utility>
#include <QString>


using Position = std::pair<double, double>;

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

    void SetStartPoint(double lat, double lng);
    double StartLat();
    double StartLng();

    void SetEndPoint(double lat, double lng);
    double EndLat();
    double EndLng();

    QString RouteColor();
    void SetRouteColor(QString& color);

private:
    Position start_route_point_;
    Position end_route_point_;
    QString route_color_ = "#008000"; //green

signals:
    void add_route_();

};

#endif // ROUTEMODEL_H
