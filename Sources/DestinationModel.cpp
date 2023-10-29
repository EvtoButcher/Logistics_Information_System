#include "Headers/DestinationModel.h"

DestinationModel::DestinationModel(QObject *parent)
    : QObject(parent)
{

}

void DestinationModel::setDestination(const DestinationInfo &new_destination)
{
    info_ = new_destination;
}

double DestinationModel::lat()
{
    return info_.position_.latitude();
}

double DestinationModel::lng()
{
    return info_.position_.longitude();
}

void DestinationModel::setDestinationStatus(int current_status)
{
     destination_status_ = static_cast<UploadDestinationStatus>(current_status);
}

UploadDestinationStatus DestinationModel::checkDestinationStatus()
{
    if(destination_status_ == UploadDestinationStatus::Colpleted) {//TODO add UploadStatus::Error handling
           destination_status_ = UploadDestinationStatus::Null;
           return UploadDestinationStatus::Colpleted;
    }
    return destination_status_;
}

DestinationInfo::DestinationInfo(const uint64_t code, const QGeoCoordinate pos)
    : code_(code)
    , position_(pos)
{}
