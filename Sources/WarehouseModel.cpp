

#include "Headers/WarehouseModel.h"


WarehouseModel::WarehouseModel(QObject *parent)
    : QObject(parent)
{

}

void WarehouseModel::setWarehouse(const WarehouseInfo &new_warehouse)
{
    info_ = new_warehouse;
}

double WarehouseModel::lat()
{
    return info_.position_.latitude();
}

double WarehouseModel::lng()
{
    return info_.position_.longitude();
}

WarehouseInfo::WarehouseInfo(const uint64_t code, const QGeoCoordinate &pos)
    : code_(code)
    , position_(pos)
{

}
