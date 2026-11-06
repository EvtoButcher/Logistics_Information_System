#ifndef CARSIMULATORMODEL_H
#define CARSIMULATORMODEL_H

#include <QSet>
#include <QTimer>

#include "AbstractTrafficModel.h"

class CarSimulatorModel : public AbstractTrifficModel
{
    Q_OBJECT

public:
    explicit CarSimulatorModel(QObject* parent = nullptr);

    QString              trafficColor()      const override;
    double               posLat()            const override;
    double               posLng()            const override;
    UploadTrifficStatus  checkUploadStatus()       override;
    TravelStatus         checkTravelStatus()       override;

    void                 addTraffic(const TrafficInfo& info) override;
    Q_INVOKABLE void     setUploadStatus(int current_status) override;
    Q_INVOKABLE void     setTravelStatus(int current_status) override;

    void                 startSimulation(int simulation_speed) override;
    void                 stopSimulation() override;
    void                 resetSimulation() override;
    void                 setSimulationSpeed(int simulation_speed) override;

private slots:
    void tick();

private:
    TrafficInfo          traffic_model_;
    QVector<TrafficInfo> initial_traffic_;
    QVector<int>         qml_cars_;
    QSet<int>            finished_;
    QTimer               timer_;
    int                  speed_ = 1;
};

#endif // CARSIMULATORMODEL_H
