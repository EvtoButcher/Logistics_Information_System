#include "RouteModel.h"

RouteModel::RouteModel(QObject *parent)
    : QObject{parent}
{

}

void RouteModel::SetStartPoint(double lat, double lng)
{
    start_route_point_ = std::make_pair(lat, lng);
}

double RouteModel::StartLat()
{
    return start_route_point_.first;
}

double RouteModel::StartLng()
{
    return start_route_point_.second;
}

void RouteModel::SetEndPoint(double lat, double lng)
{
    end_route_point_ = std::make_pair(lat, lng);
}

double RouteModel::EndLat()
{
    return end_route_point_.first;
}

double RouteModel::EndLng()
{
    return end_route_point_.second;
}

//newRoute.startPosLat = 55.908961;
//newRoute.startPosLng = 37.391218;
//newRoute.endPosLat = 55.9833043;
//newRoute.endPosLng = 37.2106466;
