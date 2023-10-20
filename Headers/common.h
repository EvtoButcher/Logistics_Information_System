#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QGeoCoordinate>

namespace common {

bool fileExists(QString path);

QGeoCoordinate splitCoordinates(const QString& input);

void delay();

}

#endif // COMMON_H