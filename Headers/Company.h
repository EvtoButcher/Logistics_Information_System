#ifndef COMPANY_H
#define COMPANY_H

#include <QString>
#include <QGeoCoordinate>
#include <QVector>
#include <deque>
#include <unordered_map>

struct Curgo
{
    explicit Curgo(const double weight, const int volume);

    uint64_t code_;
    double weight_;
    int    volume_;
};



class Warehouse
{
public:
    explicit Warehouse(const QGeoCoordinate& pos);
    explicit Warehouse(const uint64_t code, const QGeoCoordinate& pos);
    ~Warehouse();

    void addCurgo(Curgo *curgo);
    void loadCurgo();

    uint64_t getCode() const;
    const QGeoCoordinate& getPosition() const;
    const std::unordered_map<uint64_t, Curgo*> getCurgo() const;
    const Curgo* curgoAt(const uint64_t code);
    bool isValid() const;

private:
    uint64_t code_ = 0;
    QGeoCoordinate position_;

    std::unordered_map<uint64_t, Curgo*> map_of_curgo_;
};

class Destination
{
public:
    explicit Destination(const QGeoCoordinate& pos);
    explicit Destination(const uint64_t code, const QGeoCoordinate& pos);

    uint64_t getCode() const;
    const QGeoCoordinate& getPosition() const;
    bool isValid() const;

private:
    uint64_t code_ = 0;
    QGeoCoordinate position_;
};


class ApplicationSettings;

class Company
{
public:
    explicit Company() = default;
    ~Company();

    void setName(const QString& name);
    void addWarehouse(Warehouse *warehouse);
    void addDestination(Destination* destination);

    const QString getName() const;

    void restorCompany(const ApplicationSettings& setting);

private:
    QString company_name_;

    QVector<Warehouse*> warehouses_;
    QVector<Destination*> destinations_;
};

void loadCompanyFromSetting();

#endif // COMPANY_H
