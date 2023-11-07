#include <QFileInfo>
#include <QTime>
#include <QCoreApplication>
#include <QStringList>

#include "common.h"

bool common::fileExists(const QString& path) {
    QFileInfo check_file(path);
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}

QGeoCoordinate common::splitCoordinates(const QString& input) {
    QStringList parts = input.split(" ");
    return QGeoCoordinate(parts[0].toDouble(), parts[1].toDouble());
}

void common::delay()
{
    QTime dieTime= QTime::currentTime().addMSecs(10);
    while (QTime::currentTime() < dieTime){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

