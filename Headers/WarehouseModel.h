#ifndef WAREHOUSEMODEL_H
#define WAREHOUSEMODEL_H

#include <QObject>
#include <QGeoCoordinate>

#define CONTEXT_WAREHOUSE_NAME "warehouseModel"

struct WarehouseInfo
{
    WarehouseInfo() = default;
    WarehouseInfo(const uint64_t code, const QGeoCoordinate& pos);

    uint64_t code_ = 0;
    QGeoCoordinate position_;
};

class WarehouseModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double  Lat   READ lat)
    Q_PROPERTY(double  Lng   READ lng)

public:
    explicit WarehouseModel(QObject *parent = nullptr);

    void setWarehouse(const WarehouseInfo& new_warehouse);

    double lat();
    double lng();

signals:
    void addWarehouse();
    void restorWarehouse();

private:
    WarehouseInfo info_;
};

#endif // WAREHOUSEMODEL_H
