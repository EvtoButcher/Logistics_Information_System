#include "common.h"

#include <QFileInfo>
#include <QStringList>

bool fileExists(QString path) {
    QFileInfo check_file(path);
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}

std::pair<double, double> splitCoordinates(const QString& input) {
    QStringList parts = input.split(" ");
    return std::make_pair(parts[0].toDouble(), parts[1].toDouble());
}
