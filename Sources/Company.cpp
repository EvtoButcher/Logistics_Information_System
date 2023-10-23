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

uint64_t Warehouse::getCode()
{
    return code_;
}

bool Warehouse::isValid() const
{
    return code_ && position_.isValid();
}

Destination::Destination(const QString &name)
    : name_(name)
{

}

Curgo::Curgo(const QString &code, const double weight, const int volume)
    : code_(code)
    , weight_(weight)
    , volume_(volume)
{

}
