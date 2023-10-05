#ifndef ROUTEMODEL_H
#define ROUTEMODEL_H

#include <QObject>
#include <utility>


using Position = std::pair<double, double>;

class RouteModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double StartLat READ StartLat)
    Q_PROPERTY(double StartLng READ StartLng)
    Q_PROPERTY(double EndLat READ EndLat)
    Q_PROPERTY(double EndLng READ EndLng)

public:
    explicit RouteModel(QObject *parent = nullptr);

    void SetStartPoint(double lat, double lng);
    double StartLat();
    double StartLng();

    void SetEndPoint(double lat, double lng);
    double EndLat();
    double EndLng();

private:
    Position start_route_point_;
    Position end_route_point_;

signals:
    void add_route_();

};

#endif // ROUTEMODEL_H
