#ifndef ABSTRACTCOMPANYITEMMODEL_H
#define ABSTRACTCOMPANYITEMMODEL_H

#include <QGeoCoordinate>

class AbstractCompanyItem
{
public:
    AbstractCompanyItem() = default;
    explicit AbstractCompanyItem(const QGeoCoordinate position);
    explicit AbstractCompanyItem(const uint64_t code, const QGeoCoordinate pos);

    uint64_t getCode() const;
    QGeoCoordinate getPosition() const;
    bool isValid() const;

private:
    uint64_t       code_ = 0;
    QGeoCoordinate position_;
};

namespace company_item {
    using Destination = AbstractCompanyItem;
}

#endif // ABSTRACTCOMPANYITEMMODEL_H
