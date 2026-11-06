#include <QStringList>
#include <QCryptographicHash>

#include "common.h"

namespace {

uint64_t codeFromKey(const QByteArray& key)
{
    const QByteArray hash = QCryptographicHash::hash(key, QCryptographicHash::Sha256);

    uint64_t code = 0;
    for(int index = 0; index < static_cast<int>(sizeof(code)); ++index) {
        code = (code << 8) | static_cast<unsigned char>(hash.at(index));
    }

    return code;
}

} // namespace

QGeoCoordinate common::splitCoordinates(const QString& input)
{
    const QStringList parts = input.split(" ", Qt::SkipEmptyParts);
    if(parts.size() < 2) {
        return QGeoCoordinate();
    }

    bool latitudeOk = false;
    bool longitudeOk = false;
    const double latitude = parts[0].toDouble(&latitudeOk);
    const double longitude = parts[1].toDouble(&longitudeOk);

    if(!latitudeOk || !longitudeOk) {
        return QGeoCoordinate();
    }

    return QGeoCoordinate(latitude, longitude);
}

uint64_t common::createCode(const QGeoCoordinate& coordinate)
{
    const QByteArray key = QStringLiteral("%1/%2")
                               .arg(QString::number(coordinate.latitude(), 'f', 6),
                                    QString::number(coordinate.longitude(), 'f', 6))
                               .toUtf8();

    return codeFromKey(key);
}

uint64_t common::createCode(double weight, int volume)
{
    const QByteArray key = QStringLiteral("%1/%2")
                               .arg(QString::number(weight, 'f', 6),
                                    QString::number(volume))
                               .toUtf8();

    return codeFromKey(key);
}
