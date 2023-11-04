#include "AbstractTrafficModel.h"

AbstractTrifficModel::AbstractTrifficModel(QObject* parent)
    : QStateMachine(parent)
{
    initializing_ = new QState(this);
    waiting_      = new QState(this);
    on_way_       = new QState(this);
    finished_     = new QState(this);
    arrived_      = new QState(this);

    // Initial states
    setInitialState(initializing_);

    // Transitions
    initializing_   ->addTransition(this, &AbstractTrifficModel::next_state,  waiting_);
    waiting_        ->addTransition(this, &AbstractTrifficModel::next_state,  on_way_);
    on_way_         ->addTransition(this, &AbstractTrifficModel::next_state,  arrived_);
    arrived_        ->addTransition(this, &AbstractTrifficModel::next_state,  on_way_);
    arrived_        ->addTransition(this, &AbstractTrifficModel::next_state,  finished_);

    //Connect slots
    connect(initializing_, &QState::entered, this, &AbstractTrifficModel::onInitializing);
    connect(waiting_     , &QState::entered, this, &AbstractTrifficModel::onWaiting);
    connect(on_way_      , &QState::entered, this, &AbstractTrifficModel::onOnWay);
    connect(arrived_     , &QState::entered, this, &AbstractTrifficModel::onArrived);
    connect(finished_    , &QState::entered, this, &AbstractTrifficModel::onFinished);
}

AbstractTrifficModel::~AbstractTrifficModel()
{
    stop();
}


void AbstractTrifficModel::addTraffic(const TrafficInfo& info)
{
    traffic_.push_back(info);
}

QVector<TrafficInfo>& AbstractTrifficModel::getTraffic()
{
    return traffic_;
}

bool AbstractTrifficModel::arrivedAtDestination(int index)
{
    return traffic_[index].travel_Status_ == TravelStatus::Arrived;
}

TrafficInfo::TrafficInfo(const QVector<QGeoCoordinate>& path, const QString& color)
    : color_(color)
    , path_(path)
{

}
