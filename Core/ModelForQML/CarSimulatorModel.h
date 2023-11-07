#ifndef CARSIMULATORMODEL_H
#define CARSIMULATORMODEL_H

#include <QVector>
#include <QVariantList>

#include "AbstractTrafficModel.h"

class CarSimulatorModel : public AbstractTrifficModel
{
    Q_OBJECT

public:
    explicit CarSimulatorModel(QObject* parent = nullptr);
    ~CarSimulatorModel() = default;

    QString              trafficColor()      override;
    double               posLat()            override;
    double               posLng()            override;
    UploadTrifficStatus  checkUploadStatus() override;
    TravelStatus         checkTravelStatus() override;

    void                 addTraffic(const TrafficInfo& info) override;
    Q_INVOKABLE void     setUploadStatus(int current_status /*UploadTrifficStatus*/) override;
    Q_INVOKABLE void     setTravelStatus(int current_status /*TravelStatus*/)        override;

protected slots:
    void onInitializing() override;
    void onWaiting()      override;
    void onOnWay()        override;
    void onArrived()      override;
    void onFinished()     override;

private:
    void startSimulation(int simulation_speed);

    TrafficInfo traffic_model_;
};

#endif // CARSIMULATORMODEL_H
