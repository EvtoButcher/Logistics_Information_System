#include <QDebug>

#include "CarSimulatorModel.h"

CarSimulatorModel::CarSimulatorModel(QObject* parent)
    : AbstractTrifficModel(parent)
{
    timer_.setInterval(500);
    connect(&timer_, &QTimer::timeout, this, &CarSimulatorModel::tick);
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
    return UploadTrifficStatus::Colpleted;
}

TravelStatus CarSimulatorModel::checkTravelStatus()
{
    return traffic_model_.travel_Status_;
}

void CarSimulatorModel::addTraffic(const TrafficInfo& info)
{
    AbstractTrifficModel::addTraffic(info);
    initial_traffic_.push_back(info);
}

void CarSimulatorModel::setUploadStatus(int current_status)
{
    Q_UNUSED(current_status)
}

void CarSimulatorModel::setTravelStatus(int current_status)
{
    traffic_model_.travel_Status_ = static_cast<TravelStatus>(current_status);
}

void CarSimulatorModel::startSimulation(int simulation_speed)
{
    if(initial_traffic_.isEmpty()) {
        qDebug() << "Simulation has no routes";
        return;
    }

    stopSimulation();
    setSimulationSpeed(simulation_speed);

    traffic_ = initial_traffic_;
    finished_.clear();
    qml_cars_.clear();
    emit clearCars();

    for(int index = 0; index < traffic_.size(); ++index) {
        if(traffic_[index].path_.isEmpty()) {
            finished_.insert(index);
            continue;
        }

        traffic_[index].travel_Status_ = TravelStatus::On_Way;
        traffic_model_ = traffic_[index];
        qml_cars_.push_back(index);
        emit addCar();
    }

    if(finished_.size() == traffic_.size()) {
        emit simulationFinished();
        return;
    }

    qDebug() << "Simulation started with" << qml_cars_.size() << "car(s)";
    timer_.start();
}

void CarSimulatorModel::stopSimulation()
{
    timer_.stop();
}

void CarSimulatorModel::resetSimulation()
{
    stopSimulation();
    finished_.clear();
    qml_cars_.clear();
    traffic_ = initial_traffic_;
    traffic_model_ = TrafficInfo();
    emit clearCars();
}

void CarSimulatorModel::setSimulationSpeed(int simulation_speed)
{
    speed_ = qBound(1, simulation_speed, 10);
    timer_.setInterval(500 / speed_);
}

void CarSimulatorModel::tick()
{
    bool hasActiveCars = false;

    for(int qmlIndex = 0; qmlIndex < qml_cars_.size(); ++qmlIndex) {
        const int index = qml_cars_[qmlIndex];
        if(finished_.contains(index)) {
            continue;
        }

        QVector<QGeoCoordinate>& path = traffic_[index].path_;
        if(path.size() <= 1) {
            traffic_[index].travel_Status_ = TravelStatus::Arrived;
            finished_.insert(index);
            continue;
        }

        path.pop_front();
        traffic_model_ = traffic_[index];
        emit nextPoint(qmlIndex);
        hasActiveCars = true;
    }

    if(!hasActiveCars) {
        stopSimulation();
        qDebug() << "Simulation finished";
        emit simulationFinished();
    }
}
