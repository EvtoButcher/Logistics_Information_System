#include "Company.h"

#include <QSqlTableModel>
#include <QtAlgorithms>

#include "OrderDB.h"
#include "common.h"

Company::Company(QObject* parent)
   : QObject(parent)
{
}

Company::~Company()
{
    qDeleteAll(warehouses_);
    qDeleteAll(destinations_);
}

void Company::setName(const QString& name)
{
    company_name_ = name;
}

void Company::addWarehouse(Warehouse* warehouse)
{
    warehouses_.push_back(warehouse);
}

void Company::addDestination(company_item::Destination* destination)
{
    destinations_.push_back(destination);
}

bool Company::removeWarehouse(uint64_t code)
{
    for(auto iterator = warehouses_.begin(); iterator != warehouses_.end(); ++iterator) {
        if((*iterator)->getCode() == code) {
            delete *iterator;
            warehouses_.erase(iterator);
            return true;
        }
    }

    return false;
}

bool Company::removeDestination(uint64_t code)
{
    for(auto iterator = destinations_.begin(); iterator != destinations_.end(); ++iterator) {
        if((*iterator)->getCode() == code) {
            delete *iterator;
            destinations_.erase(iterator);
            return true;
        }
    }

    return false;
}

const QString Company::getName() const
{
    return company_name_;
}

void Company::restorCompany(const OrderDB* route_db)
{
     QSqlTableModel table_model(nullptr, route_db->DB());

     table_model.setTable(WAREHOUSE_TABLE);
     table_model.select();

     if(table_model.rowCount()) {
         warehouses_.reserve(table_model.rowCount());
         for (int row = 0; row < table_model.rowCount(); ++row) {

             const uint64_t code = table_model.data(table_model.index(row, 1)).toULongLong();
             auto* warehouse = new Warehouse(code,
                                    common::splitCoordinates(table_model.data(table_model.index(row, 2)).toString()));

             const QVector<CargoRecord> cargo = route_db->selectCargo(QString::number(code));
             for(const auto& record : cargo) {
                 warehouse->addCargo(new Cargo(record.code_, record.weight_, record.volume_));
             }

             addWarehouse(warehouse);
             emit addedNewWarehouse(warehouse);
         }
     }

     table_model.setTable(DESTINATION_TABLE);
     table_model.select();

     if(table_model.rowCount()) {
         destinations_.reserve(table_model.rowCount());
         for (int row = 0; row < table_model.rowCount(); ++row) {

             auto* destination = new company_item::Destination(
                 table_model.data(table_model.index(row, 1)).toULongLong(),
                 common::splitCoordinates(table_model.data(table_model.index(row, 2)).toString()));

             addDestination(destination);
             emit addedNewDestination(destination);
         }
     }
}

Warehouse::Warehouse(const QGeoCoordinate pos)
    : AbstractCompanyItem(pos)
{
}

Warehouse::Warehouse(const uint64_t code, const QGeoCoordinate pos)
    : AbstractCompanyItem(code, pos)
{
}

Warehouse::~Warehouse()
{
    for(auto [_, curgo_ptr] : map_of_curgo_){
        delete curgo_ptr;
    }
}

void Warehouse::addCargo(Cargo* curgo)
{
    map_of_curgo_[curgo->code_] = curgo;
}

const std::unordered_map<uint64_t, Cargo*>& Warehouse::getCargo() const
{
    return map_of_curgo_;
}

Cargo::Cargo(const double weight, const int volume)
    : code_(common::createCode(weight, volume))
    , weight_(weight)
    , volume_(volume)
{
}

Cargo::Cargo(const uint64_t code, const double weight, const int volume)
    : code_(code)
    , weight_(weight)
    , volume_(volume)
{
}
