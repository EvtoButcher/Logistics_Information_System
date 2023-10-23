#ifndef COMPANY_H
#define COMPANY_H

#include <QString>
#include <QGeoCoordinate>
#include <QVector>
#include <deque>
#include <unordered_map>

struct Curgo
{
    explicit Curgo(const QString& code, const double weight, const int volume);

    QString code_;
    double weight_;
    int    volume_;
};

class Warehouse
{
public:
    explicit Warehouse(const QGeoCoordinate& pos);
    ~Warehouse();

    void addCurgo(Curgo *curgo);
    void loadCurgo();

    uint64_t getCode();
    bool isValid() const;

private:
    uint64_t code_ = 0;
    QGeoCoordinate position_;

    std::unordered_map<QString, Curgo*> map_of_curgo_;
};

class Destination
{
    Destination(const QString& name);

private:
    QString name_;
};

class Company
{
public:
    explicit Company() = default;
    ~Company();

    void setName(const QString& name);
    void addWarehouse(Warehouse *warehouse);
    void addDestination(Destination* destination);

private:
    QString company_name_;

    QVector<Warehouse*> warehouses_;
    QVector<Destination*> destinations_;
};

void loadCompanyFromSetting();

#endif // COMPANY_H
