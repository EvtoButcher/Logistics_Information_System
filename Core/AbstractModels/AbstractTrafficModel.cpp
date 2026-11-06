#include "AbstractTrafficModel.h"

AbstractTrifficModel::AbstractTrifficModel(QObject* parent)
    : QObject(parent)
{
}

void AbstractTrifficModel::addTraffic(const TrafficInfo& info)
{
    traffic_.push_back(info);
}

QVector<TrafficInfo>& AbstractTrifficModel::getTraffic()
{
    return traffic_;
}

bool AbstractTrifficModel::arrivedAtDestination(int index) const
{
    if(index < 0 || index >= traffic_.size()) {
        return false;
    }

    return traffic_[index].travel_Status_ == TravelStatus::Arrived;
}

bool AbstractTrifficModel::trafficIsEmpty() const
{
    return traffic_.isEmpty();
}

TrafficInfo::TrafficInfo(const QVector<QGeoCoordinate>& path, const QString& color)
    : color_(color)
    , path_(path)
{
}
