#include "AbstractMapItemModel.h"

AbstractMapPoint::AbstractMapPoint(uint64_t code, QGeoCoordinate position)
    : code_(code)
    , position_(position)
{
}

//AbstractMapPoint::AbstractMapPoint(const QGeoCoordinate& position)
//    : position_(position)
//{
//}

double AbstractMapPoint::getLatitude() const
{
    return position_.latitude();
}

double AbstractMapPoint::getLongitude() const
{
    return position_.longitude();
}
