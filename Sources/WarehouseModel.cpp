

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

void WarehouseModel::setWarehouseStatus(int current_status)
{
     warehouse_status_ = static_cast<UploadWarehouseStatus>(current_status);
}

UploadWarehouseStatus WarehouseModel::checkWarehouseStatus()
{
    if(warehouse_status_ == UploadWarehouseStatus::Colpleted) {//TODO add UploadStatus::Error handling
           warehouse_status_ = UploadWarehouseStatus::Null;
           return UploadWarehouseStatus::Colpleted;
    }
    return warehouse_status_;
}

WarehouseInfo::WarehouseInfo(const uint64_t code, const QGeoCoordinate pos)
    : code_(code)
    , position_(pos)
{

}
