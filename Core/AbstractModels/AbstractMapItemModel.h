#ifndef ABSTRACTMAPITEMMODEL_H
#define ABSTRACTMAPITEMMODEL_H

#include <QGeoCoordinate>

class AbstractMapPoint {
public:
    AbstractMapPoint() = default;
    AbstractMapPoint(const uint64_t code, const QGeoCoordinate position);

    double getLatitude()  const;
    double getLongitude() const;

private:
    uint64_t       code_ = 0;
    QGeoCoordinate position_;
};

namespace map_Item_Info
{
    using DestinationInfo = AbstractMapPoint;
    using WarehouseInfo = AbstractMapPoint;
}

#endif // ABSTRACTMAPITEMMODEL_H
