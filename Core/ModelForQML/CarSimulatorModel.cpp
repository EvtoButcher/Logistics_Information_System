#include <QDebug>

#include "CarSimulatorModel.h"
#include "common.h"


CarSimulatorModel::CarSimulatorModel(QObject* parent)
    : AbstractTrifficModel(parent)
{
    connect(this, &AbstractTrifficModel::finished_state, this, &CarSimulatorModel::addIgnore);
}

QString CarSimulatorModel::trafficColor() const
{
    return traffic_model_.color_;
}


double CarSimulatorModel::posLat() const
{
    if(traffic_model_.path_.isEmpty()) {
        return 0.0;
    }

    return traffic_model_.path_.front().latitude();
}

double CarSimulatorModel::posLng() const
{
    if(traffic_model_.path_.isEmpty()) {
        return 0.0;
    }

    return traffic_model_.path_.front().longitude();
}

UploadTrifficStatus CarSimulatorModel::checkUploadStatus()
{
    if(traffic_model_.upload_status_ == UploadTrifficStatus::Colpleted) {//TODO add UploadStatus::Error handling
               traffic_model_.upload_status_ = UploadTrifficStatus::Null;
               return UploadTrifficStatus::Colpleted;
        }

    return traffic_model_.upload_status_;
}

TravelStatus CarSimulatorModel::checkTravelStatus()
{
    return traffic_model_.travel_Status_;
}

void CarSimulatorModel::addTraffic(const TrafficInfo& info)
{
    ::AbstractTrifficModel::addTraffic(info);
}

void CarSimulatorModel::setUploadStatus(int current_status)
{
    traffic_model_.upload_status_ = static_cast<UploadTrifficStatus>(current_status);
}

void CarSimulatorModel::setTravelStatus(int current_status)
{
    traffic_model_.travel_Status_ = static_cast<TravelStatus>(current_status);
}


void CarSimulatorModel::onInitializing()
{
   auto& traffic = getTraffic();
   for(int i = 0; i < traffic.size(); ++i){
       traffic_model_ = traffic[i];
       emit addCar();
   }

   emit next_state();
}

void CarSimulatorModel::onWaiting()
{
    //TODO: Error hendeling
    qDebug() << "Waiting";
    emit next_state();
}

void CarSimulatorModel::onOnWay()
{
    qDebug() << "OnWay";
    auto& traffic = getTraffic();
    for(int i = 0; i < traffic.size(); ++i){

        if(ignore_traffic_.find(i) != ignore_traffic_.end()) {
            continue;
        }

        if(traffic[i].path_.isEmpty()) {
            emit finished_state(i);
            continue;
        }

        traffic[i].path_.pop_front();
        traffic_model_ = traffic[i];

        if(traffic_model_.path_.isEmpty()) {
             emit finished_state(i);
             continue;
        }

        emit nextPoint(i);

        while(checkUploadStatus() != UploadTrifficStatus::Colpleted);
        common::delay();
    }

    emit next_state();
}

void CarSimulatorModel::onArrived()
{
    qDebug() << "Arrived";
    emit next_state();
}

void CarSimulatorModel::onFinished()
{
    qDebug() <<"Finished";
    emit next_state();
}

void CarSimulatorModel::addIgnore(int index)
{
    ignore_traffic_.insert(index);
    qDebug() << QString("car %1 Arrived").arg(index);
}
