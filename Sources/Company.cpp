#include "Headers/Company.h"

Company::~Company()
{
    for(auto warehouse : warehouses_){ // MB delete []
        warehouse->~Warehouse();
        delete warehouse;
    }

    for(auto destination : destinations_){
        delete destination;
    }
}

void Company::setName(const QString &name)
{
    company_name_ = name;
}

void Company::addWarehouse(Warehouse *warehouse)
{
    warehouses_.push_back(warehouse);
}

void Company::addDestination(Destination *destination)
{
    destinations_.push_back(destination);
}

const QString Company::getName() const
{
    return company_name_;
}

Warehouse::Warehouse(const QGeoCoordinate &pos)
    : position_(pos)
{
    double tmp_lat = position_.latitude();
    double tmp_lng = position_.longitude();
    uint64_t hash_lat = *reinterpret_cast<uint64_t*>(&tmp_lat);
    uint64_t hash_lng = *reinterpret_cast<uint64_t*>(&tmp_lng);

    code_ = hash_lat ^ hash_lng;
}

Warehouse::~Warehouse()
{
    for(auto [_, curgo_ptr] : map_of_curgo_){
        delete curgo_ptr;
    }
}

void Warehouse::addCurgo(Curgo *curgo)
{
    map_of_curgo_[curgo->code_] = curgo;
}

uint64_t Warehouse::getCode() const
{
    return code_;
}

const QGeoCoordinate &Warehouse::getPosition() const
{
    return position_;
}

const std::unordered_map<uint64_t, Curgo *> Warehouse::getCurgo() const
{
    return map_of_curgo_;
}

bool Warehouse::isValid() const
{
    return code_ && position_.isValid();
}

Destination::Destination(const QGeoCoordinate &pos)
{
    double tmp_lat = pos.latitude();
    double tmp_lng = pos.longitude();
    uint64_t hash_lat = *reinterpret_cast<uint64_t*>(&tmp_lat);
    uint64_t hash_lng = *reinterpret_cast<uint64_t*>(&tmp_lng);

    code_ = hash_lat ^ hash_lng;
}

uint64_t Destination::getCode() const
{
    return code_;
}

const QGeoCoordinate &Destination::getPosition() const
{

}

Curgo::Curgo(const double weight, const int volume)
    : weight_(weight)
    , volume_(volume)
{
    double tmp_weight = weight;
    double tmp_volume = volume;
    uint64_t hash_weight = *reinterpret_cast<uint64_t*>(&tmp_weight);
    uint64_t hash_volume = *reinterpret_cast<uint64_t*>(&tmp_volume);

    code_ = hash_weight ^ hash_volume;
}
