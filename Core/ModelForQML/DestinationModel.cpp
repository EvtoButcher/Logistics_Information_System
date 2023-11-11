#include "DestinationModel.h"

DestinationModel::DestinationModel(QObject* parent)
    : QObject(parent)
{
}

void DestinationModel::setDestination(const map_Item_Info::DestinationInfo& new_destination)
{
    info_ = new_destination;
}

double DestinationModel::lat() const
{
    return info_.getLatitude();
}

double DestinationModel::lng() const
{
    return info_.getLongitude();
}

void DestinationModel::setDestinationStatus(int current_status)
{
     destination_status_ = static_cast<UploadDestinationStatus>(current_status);
}

UploadDestinationStatus DestinationModel::checkDestinationStatus()
{
    if(destination_status_ == UploadDestinationStatus::Colpleted) { // TODO add UploadStatus::Error handling
           destination_status_ = UploadDestinationStatus::Null;
           return UploadDestinationStatus::Colpleted;
    }

    return destination_status_;
}
