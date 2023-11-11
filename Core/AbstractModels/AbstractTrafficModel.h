#ifndef ABSTRACTTRAFFICMODEL_H
#define ABSTRACTTRAFFICMODEL_H

#include <QObject>
#include <QVector>
#include <QGeoCoordinate>
#include <QStateMachine>

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

class AbstractTrifficModel : public QStateMachine
{
    Q_OBJECT
    Q_PROPERTY(double    PosLat    READ  posLat)
    Q_PROPERTY(double    PosLng    READ  posLng)
    Q_PROPERTY(QString   CarColor  READ  trafficColor)

public:
    explicit AbstractTrifficModel(QObject* parent = nullptr);
    ~AbstractTrifficModel();

    bool                         arrivedAtDestination(int index);
    void                         setTraffic(const QVector<TrafficInfo>& tr);
    bool                         trafficIsEmpty();
    QVector<TrafficInfo>&        getTraffic();

    virtual void                 addTraffic(const TrafficInfo& info);
    virtual QString              trafficColor()      = 0;
    virtual double               posLat()            = 0;
    virtual double               posLng()            = 0;
    virtual UploadTrifficStatus  checkUploadStatus() = 0;
    virtual TravelStatus         checkTravelStatus() = 0;

    virtual Q_INVOKABLE void     setUploadStatus(int current_status /*UploadTrifficStatus*/) = 0;
    virtual Q_INVOKABLE void     setTravelStatus(int current_status /*TravelStatus*/)        = 0;

signals:
    void next_state();
    void finished_state(int);

    void addCar();
    void nextPoint(int);
    void arrived();
    void finished();

    //void restoreTraffic();

protected slots:
    virtual void onInitializing()   = 0;
    virtual void onWaiting()        = 0;
    virtual void onOnWay()          = 0;
    virtual void onArrived()        = 0;
    virtual void onFinished()       = 0;

private:
    QState* initializing_;
    QState* waiting_;
    QState* on_way_;
    QState* arrived_;
    QState* finished_;

    QVector<TrafficInfo> traffic_;
};

#endif // ABSTRACTTRAFFICMODEL_H
