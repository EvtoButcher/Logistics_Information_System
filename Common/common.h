#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QVector>
#include <QGeoCoordinate>

namespace common {

template<typename... Args>
uint64_t createCode(Args... args)
{
    if constexpr(sizeof...(args) == 1) {

        double tmp_lat = ((static_cast<QGeoCoordinate>(args).latitude()), ...);
        double tmp_lng = ((static_cast<QGeoCoordinate>(args).longitude()), ...);

        uint64_t hash_lat = *reinterpret_cast<uint64_t*>(&tmp_lat);
        uint64_t hash_lng = *reinterpret_cast<uint64_t*>(&tmp_lng);

        return  hash_lat ^ hash_lng;
    }
    else if constexpr(sizeof...(args) == 2) {

       std::tuple<Args...> tupleArgs(args...);
       double tmp_weight = std::get<0>(tupleArgs);
       double tmp_volume = std::get<1>(tupleArgs);

       uint64_t hash_weight = *reinterpret_cast<uint64_t*>(&tmp_weight);
       uint64_t hash_volume = *reinterpret_cast<uint64_t*>(&tmp_volume);

       return hash_weight ^ hash_volume;
    }
}

bool fileExists(const QString& path);

QGeoCoordinate splitCoordinates(const QString& input);

void delay();

}

#endif // COMMON_H
