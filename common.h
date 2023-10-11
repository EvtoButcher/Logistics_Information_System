#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <utility>

bool fileExists(QString path);

std::pair<double, double> splitCoordinates(const QString& input);

#endif // COMMON_H
