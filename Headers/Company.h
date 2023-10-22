#ifndef COMPANY_H
#define COMPANY_H

#include <QString>
#include <QGeoCoordinate>
#include <QVector>
#include <deque>
#include <unordered_map>

struct Curgo
{
    Curgo(const QString& code, const double weight, const int volume);

    QString code_;
    double weight_;
    int    volume_;
};

class Warehouse
{
public:
    Warehouse(const QString& code, const QGeoCoordinate& pos);

    void addCurgo(const Curgo& curgo);

    void loadCurgo();

private:
    QString code_;
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
    Company(const QString& name);

    void addWarehouse(const Warehouse* warehouse);
    void addDestination(const Destination* destination);

private:
    QString company_name_;

    QVector<Warehouse*> warehouses_;
    QVector<Destination*> destonations_;
};

void loadCompanyFromSetting();

#endif // COMPANY_H
