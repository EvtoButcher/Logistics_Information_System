#include "RouteModel.h"

RouteModel::RouteModel(QObject *parent)
    : QObject{parent}
{

}

void RouteModel::setRoute(const RouteInfo &newRoute)
{
    route = newRoute;
}

double RouteModel::StartLat()
{
    return route.start_route_point_.first;
}

double RouteModel::StartLng()
{
    return route.start_route_point_.second;
}


double RouteModel::EndLat()
{
    return route.end_route_point_.first;
}

double RouteModel::EndLng()
{
    return route.end_route_point_.second;
}

QString RouteModel::RouteColor()
{
    return route.route_color_;
}

RouteInfo::RouteInfo(QString name, double s_lat, double s_lng, double e_lat, double e_lng, QString color)
    : name_(name)
    , route_color_(color)
{
    start_route_point_ = std::make_pair(s_lat, s_lng);
    end_route_point_   = std::make_pair(e_lat, e_lng);

    //route_color_ = color;
}
