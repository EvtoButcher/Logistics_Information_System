#ifndef DESTINATIONMODEL_H
#define DESTINATIONMODEL_H

#include <QObject>
#include <QGeoCoordinate>

#define CONTEXT_DESTINATION_NAME "destinationModel"

struct DestinationInfo
{
    DestinationInfo() = default;
    DestinationInfo(const uint64_t code, const QGeoCoordinate pos);

    uint64_t code_ = 0;
    QGeoCoordinate position_;
};

enum class UploadDestinationStatus{
    Null,
    Colpleted,
    Loading
};

class DestinationModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double  Lat   READ lat)
    Q_PROPERTY(double  Lng   READ lng)

public:
    explicit DestinationModel(QObject *parent = nullptr);

    void setDestination(const DestinationInfo& new_destination);

    double lat();
    double lng();

    Q_INVOKABLE void setDestinationStatus(int current_status /*UploadDestinationStatus*/);
    UploadDestinationStatus checkDestinationStatus();

signals:
    void addDestination();
    void restorDestination();

private:
    DestinationInfo info_;
    UploadDestinationStatus destination_status_ = UploadDestinationStatus::Null;
};

#endif // DESTINATIONMODEL_H
