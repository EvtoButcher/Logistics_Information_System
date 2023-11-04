#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QVector>
#include <QGeoCoordinate>

namespace common {

bool fileExists(const QString &path);

QGeoCoordinate splitCoordinates(const QString& input);

void delay();

}

#endif // COMMON_H
