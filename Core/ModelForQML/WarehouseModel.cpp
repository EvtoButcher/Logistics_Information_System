#include "WarehouseModel.h"


WarehouseModel::WarehouseModel(QObject* parent)
    : QObject(parent)
{

}

void WarehouseModel::setWarehouse(const map_Item_Info::WarehouseInfo& new_warehouse)
{
    info_ = new_warehouse;
}

double WarehouseModel::lat() const
{
    return info_.getLatitude();
}

double WarehouseModel::lng() const
{
    return info_.getLongitude();
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
