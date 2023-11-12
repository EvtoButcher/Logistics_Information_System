#ifndef DESTINATIONMODEL_H
#define DESTINATIONMODEL_H

#include <QObject>
#include <QGeoCoordinate>

#include "AbstractMapItemModel.h"

#define CONTEXT_DESTINATION_NAME "destinationModel"

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
    explicit DestinationModel(QObject* parent = nullptr);

    void setDestination(const map_Item_Info::DestinationInfo& new_destination);

    double lat() const;
    double lng() const;

    Q_INVOKABLE void setDestinationStatus(int current_status /*UploadDestinationStatus*/);
    UploadDestinationStatus checkDestinationStatus();

signals:
    void addDestination();
    void restorDestination();

private:
    map_Item_Info::DestinationInfo info_;
    UploadDestinationStatus destination_status_ = UploadDestinationStatus::Null;
};

#endif // DESTINATIONMODEL_H
