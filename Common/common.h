#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

#include <QString>
#include <QVector>
#include <QGeoCoordinate>

namespace common {

uint64_t createCode(const QGeoCoordinate& coordinate);
uint64_t createCode(double weight, int volume);

bool fileExists(const QString& path);

QGeoCoordinate splitCoordinates(const QString& input);

void delay();

}

#endif // COMMON_H
