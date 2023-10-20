#include <QtDebug>

#include "Headers/RouteModel.h"
#include "Headers/common.h"

RouteModel::RouteModel(QObject *parent)
    : QObject{parent}
{

}

void RouteModel::setRoute(const RouteInfo &newRoute)
{
    route_ = newRoute;
}

void RouteModel::setRouteStatus(int current_status)
{
    route_status_ = static_cast<UploadStatus>(current_status);
}

void RouteModel::setPathCacheStatus(int current_status)
{
    path_cache_status_ = static_cast<UploadStatus>(current_status);
}

UploadStatus RouteModel:: checkPathCacheStatus()
{
    if(path_cache_status_ == UploadStatus::Colpleted) {//TODO add UploadStatus::Error handling
           path_cache_status_ = UploadStatus::Null;
           return UploadStatus::Colpleted;
    }
    return route_status_;
}

UploadStatus RouteModel::checkRouteStatus()
{
    if(route_status_ == UploadStatus::Colpleted) {//TODO add UploadStatus::Error handling
           route_status_ = UploadStatus::Null;
           return UploadStatus::Colpleted;
    }
    return route_status_;
}

const RouteInfo& RouteModel::getInfo()
{
    return route_;
}

double RouteModel::startLat()
{
    return route_.start_route_point_.latitude();
}

double RouteModel::startLng()
{
    return route_.start_route_point_.longitude();
}

double RouteModel::endLat()
{
    return route_.end_route_point_.latitude();
}

double RouteModel::endLng()
{
    return route_.end_route_point_.longitude();
}

QString RouteModel::routeColor()
{
    return route_.route_color_;
}

QVariantList RouteModel::routePath()
{
    QVariantList list;

    for(const auto& coordinate : route_.path_cache_){
        list.append(QVariant::fromValue(coordinate));
    }

    return list;
}

void RouteModel::setPathCache(QJSValue path)
{
    QVariantList coordinate_list = qvariant_cast<QVariantList>(path.toVariant());

    bool isFirstIteration = true;
    double distance = 0;
    route_.path_cache_.reserve(coordinate_list.size());
    for(QVariant position : coordinate_list) {
        if(!isFirstIteration) {
            distance += route_.path_cache_.back().distanceTo(qvariant_cast<QGeoCoordinate>(position));
        }
        route_.path_cache_.append(qvariant_cast<QGeoCoordinate>(position));
        isFirstIteration = false;
    }

    route_.path_distance_ = distance / 1000;

    path_cache_status_ = UploadStatus::Colpleted;
}

RouteInfo::RouteInfo(QString name, double start_lat, double start_lng, double end_lat, double end_lng, QString color)
    : code_(name)
    , start_route_point_(QGeoCoordinate(start_lat, start_lng))
    , end_route_point_(QGeoCoordinate(end_lat, end_lng))
    , route_color_(color)
{

}

RouteInfo::RouteInfo(const QString name, const QGeoCoordinate start, const QGeoCoordinate end, const QString &path_cache, const QString color)
   : code_(name)
   , start_route_point_(start)
   , end_route_point_(end)
   , route_color_(color)
{
    //if(path_cache.isEmpty()){
    //    return;
    //}

    QStringList parts = path_cache.split(" ", Qt::SkipEmptyParts);

    const auto first_point = common::splitCoordinates(parts[0] + " " + parts[1]);
    bool isFirst = true;

    for(int i = 0; i < parts.size(); i += 2) {                                       //
        const auto& point = common::splitCoordinates(parts[i] + " " + parts[i + 1]); //TODO: FIX THIS SHIT AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
        if(!isFirst){                                                                //
            if(point == first_point){
                break;
            }
        }
        path_cache_.push_back(point);
        isFirst = false;
    }
}
