#ifndef WAREHOUSEMODEL_H
#define WAREHOUSEMODEL_H

#include <QObject>
#include <QGeoCoordinate>

#include "AbstractMapItemModel.h"

#define CONTEXT_WAREHOUSE_NAME "warehouseModel"

enum class UploadWarehouseStatus{
    Null,
    Colpleted,
    Loading
};

class WarehouseModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double  Lat  READ lat)
    Q_PROPERTY(double  Lng  READ lng)

public:
    explicit WarehouseModel(QObject* parent = nullptr);

    void setWarehouse(const map_Item_Info::WarehouseInfo& new_warehouse);

    double lat() const;
    double lng() const;

    Q_INVOKABLE void setWarehouseStatus(int current_status /*UploadWarehouseStatus*/);

    UploadWarehouseStatus checkWarehouseStatus();

signals:
    void addWarehouse();
    void restorWarehouse();

private:
    map_Item_Info::WarehouseInfo info_;
    UploadWarehouseStatus warehouse_status_ = UploadWarehouseStatus::Null;
};

#endif // WAREHOUSEMODEL_H
