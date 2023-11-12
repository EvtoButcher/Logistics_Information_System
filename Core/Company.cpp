#include "Company.h"

#include <QSqlTableModel>

#include "OrderDB.h"
#include "common.h"

Company::Company(QObject* parent)
   : QObject(parent)
{
}

Company::~Company()
{
    for(auto warehouse : warehouses_){ // MB delete []
        warehouse->~Warehouse();
        delete warehouse;
    }

    for(auto destination : destinations_){
        delete destination;
    }

    this->QObject::~QObject();
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

             addWarehouse(new Warehouse(table_model.data(table_model.index(row, 1)).toInt(),
                                    common::splitCoordinates(table_model.data(table_model.index(row, 2)).toString())));

             emit addedNewWarehouse(warehouses_.back());
         }
     }

     table_model.setTable(DESTINATION_TABLE);
     table_model.select();

     if(table_model.rowCount()) {
         destinations_.reserve(table_model.rowCount());
         for (int row = 0; row < table_model.rowCount(); ++row) {

             addDestination(new company_item::Destination(table_model.data(table_model.index(row, 1)).toInt(),
                                              common::splitCoordinates(table_model.data(table_model.index(row, 2)).toString())));

             emit addedNewDestination(destinations_.back());
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

const std::unordered_map<uint64_t, Cargo*> Warehouse::getCargo() const
{
    return map_of_curgo_;
}

Cargo::Cargo(const double weight, const int volume)
    : code_(common::createCode(weight, volume))
    , weight_(weight)
    , volume_(volume)
{
}
