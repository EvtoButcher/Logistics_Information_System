#include <QDebug>

#include "CarSimulatorModel.h"
#include "common.h"


CarSimulatorModel::CarSimulatorModel(QObject* parent)
    : AbstractTrifficModel(parent)
{

}

QString CarSimulatorModel::trafficColor()
{
    return traffic_model_.color_;
}


double CarSimulatorModel::posLat()
{
    return traffic_model_.path_.front().latitude();
}

double CarSimulatorModel::posLng()
{
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

}


void CarSimulatorModel::startSimulation(int simulation_speed)
{

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
        traffic[i].path_.pop_front();//LOL
        traffic_model_ = traffic[i];
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
    stop();
}
