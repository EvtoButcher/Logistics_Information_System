#include "RouteModel.h"
#include <QtDebug>

RouteModel::RouteModel(QObject *parent)
    : QObject{parent}
{

}

void RouteModel::setRoute(const RouteInfo &newRoute)
{
    route = newRoute;
}

void RouteModel::routeSelectedOnMap(int index)
{
    emit selectRouteOnMap(index);
}

void RouteModel::routeUnselectedOnMap(){
    emit unselectRouteOnMap();
}

void RouteModel::setStatus(int current_status)
{
    status = static_cast<UploadStatus>(current_status);
}

UploadStatus RouteModel::checkStatus()
{
    if(status == UploadStatus::Colpleted){//TODO add UploadStatus::Error handling
           status = UploadStatus::Null;
           return UploadStatus::Colpleted;
    }
    return status;
}

double RouteModel::startLat()
{
    return route.start_route_point_.first;
}

double RouteModel::startLng()
{
    return route.start_route_point_.second;
}

double RouteModel::endLat()
{
    return route.end_route_point_.first;
}

double RouteModel::endLng()
{
    return route.end_route_point_.second;
}

QString RouteModel::routeColor()
{
    return route.route_color_;
}

RouteInfo::RouteInfo(QString name, double start_lat, double start_lng, double end_lat, double end_lng, QString color)
    : name_(name)
    , route_color_(color)
{
    start_route_point_ = std::make_pair(start_lat, start_lng);
    end_route_point_   = std::make_pair(end_lat, end_lng);
}

RouteInfo::RouteInfo(QString name, Position start, Position end, QString color)
   : name_(name)
   , route_color_(color)
{
    start_route_point_ = start;
    end_route_point_   = end;
}
