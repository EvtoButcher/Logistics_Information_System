#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

#include <QString>
#include <QVector>
#include <QGeoCoordinate>

namespace common {

uint64_t createCode(const QGeoCoordinate& coordinate);
uint64_t createCode(double weight, int volume);

QGeoCoordinate splitCoordinates(const QString& input);

}

#endif // COMMON_H
