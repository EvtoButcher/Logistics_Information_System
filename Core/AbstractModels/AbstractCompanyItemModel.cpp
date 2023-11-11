#include "AbstractCompanyItemModel.h"
#include "common.h"

AbstractCompanyItem::AbstractCompanyItem(const QGeoCoordinate position)
    : code_(common::createCode(position))
    , position_(position)
{
}

AbstractCompanyItem::AbstractCompanyItem(const uint64_t code, const QGeoCoordinate pos)
    : code_(code)
    , position_(pos)
{
}

uint64_t AbstractCompanyItem::getCode() const
{
    return code_;
}

QGeoCoordinate AbstractCompanyItem::getPosition() const
{
    return position_;
}

bool AbstractCompanyItem::isValid() const
{
    return code_ && position_.isValid();
}
