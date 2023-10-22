#include "Headers/Company.h"

Company::Company(const QString &name)
    :company_name_(name)
{

}

Warehouse::Warehouse(const QString &code, const QGeoCoordinate &pos)
    : code_(code)
    , position_(pos)
{

}

Destination::Destination(const QString &name)
    : name_(name)
{

}
