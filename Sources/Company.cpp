#include <QSqlTableModel>

#include "Headers/Company.h"
#include "Headers/OrderDB.h"
#include "Headers/common.h"

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

void Company::restorCompany(const ApplicationSettings &setting)
{
     auto route_db_ = OrderDB(setting, nullptr);

     QSqlTableModel table_model(nullptr, route_db_.DB());

     table_model.setTable(WAREHOUSE_TABLE);
     table_model.select();

     if(table_model.rowCount()){
         warehouses_.reserve(table_model.rowCount());
         for (int row = 0; row < table_model.rowCount(); ++row) {

             warehouses_.push_back(new Warehouse(table_model.data(table_model.index(row, 1)).toInt(),
                                    common::splitCoordinates(table_model.data(table_model.index(row, 2)).toString())));
         }
     }

     table_model.setTable(DESTINATION_TABLE);
     table_model.select();

     if(table_model.rowCount()){
         destinations_.reserve(table_model.rowCount());
         for (int row = 0; row < table_model.rowCount(); ++row) {

             destinations_.push_back(new Destination(table_model.data(table_model.index(row, 1)).toInt(),
                                    common::splitCoordinates(table_model.data(table_model.index(row, 2)).toString())));
         }
     }
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

Warehouse::Warehouse(const uint64_t code, const QGeoCoordinate &pos)
    : code_(code)
    , position_(pos)
{

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
    : position_(pos)
{
    double tmp_lat = pos.latitude();
    double tmp_lng = pos.longitude();
    uint64_t hash_lat = *reinterpret_cast<uint64_t*>(&tmp_lat);
    uint64_t hash_lng = *reinterpret_cast<uint64_t*>(&tmp_lng);

    code_ = hash_lat ^ hash_lng;
}

Destination::Destination(const uint64_t code, const QGeoCoordinate &pos)
    : code_(code)
    , position_(pos)
{

}

uint64_t Destination::getCode() const
{
    return code_;
}

const QGeoCoordinate &Destination::getPosition() const
{
    return position_;
}

bool Destination::isValid() const
{
    return code_ && position_.isValid();
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
