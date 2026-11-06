#ifndef ABSTRACTTRAFFICMODEL_H
#define ABSTRACTTRAFFICMODEL_H

#include <QObject>
#include <QVector>
#include <QGeoCoordinate>

enum class UploadTrifficStatus{
    Null,
    Colpleted,
    Loading
};

enum class TravelStatus{
    Null,
    Waiting,
    On_Way,
    Arrived
};

struct TrafficInfo
{
    explicit TrafficInfo(const QVector<QGeoCoordinate>& path, const QString& color);
    TrafficInfo() = default;

    QString                 color_;
    QVector<QGeoCoordinate> path_;

    UploadTrifficStatus     upload_status_ = UploadTrifficStatus::Null;
    TravelStatus            travel_Status_ = TravelStatus::Null;
};

class AbstractTrifficModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double   PosLat    READ  posLat)
    Q_PROPERTY(double   PosLng    READ  posLng)
    Q_PROPERTY(QString  CarColor  READ  trafficColor)

public:
    explicit AbstractTrifficModel(QObject* parent = nullptr);
    ~AbstractTrifficModel() override = default;

    bool                         arrivedAtDestination(int index) const;
    bool                         trafficIsEmpty() const;
    QVector<TrafficInfo>&        getTraffic();

    virtual void                 addTraffic(const TrafficInfo& info);
    virtual QString              trafficColor()      const = 0;
    virtual double               posLat()            const = 0;
    virtual double               posLng()            const = 0;
    virtual UploadTrifficStatus  checkUploadStatus()       = 0;
    virtual TravelStatus         checkTravelStatus()       = 0;

    virtual void                 startSimulation(int simulation_speed) = 0;
    virtual void                 stopSimulation() = 0;
    virtual void                 resetSimulation() = 0;
    virtual void                 setSimulationSpeed(int simulation_speed) = 0;

    virtual Q_INVOKABLE void     setUploadStatus(int current_status) = 0;
    virtual Q_INVOKABLE void     setTravelStatus(int current_status) = 0;

signals:
    void addCar();
    void nextPoint(int);
    void clearCars();
    void simulationFinished();

protected:
    QVector<TrafficInfo> traffic_;
};

#endif // ABSTRACTTRAFFICMODEL_H
