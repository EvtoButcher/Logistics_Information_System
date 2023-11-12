#ifndef COMPANY_H
#define COMPANY_H

#include <QObject>
#include <QString>
#include <QGeoCoordinate>
#include <QVector>
#include <deque>
#include <unordered_map>
#include <vector>

#include "AbstractCompanyItemModel.h"

struct Cargo
{
    explicit Cargo(const double weight, const int volume);

    uint64_t code_;
    double   weight_;
    int      volume_;
};

class Warehouse : public AbstractCompanyItem
{
public:
    explicit Warehouse(const QGeoCoordinate pos);
    explicit Warehouse(const uint64_t code, const QGeoCoordinate pos);
    ~Warehouse();

    void addCargo(Cargo* curgo);
    void loadCurgo();

    const std::unordered_map<uint64_t, Cargo*> getCargo() const;
    const Cargo* cargoAt(const uint64_t code)  const;

private:
    std::unordered_map<uint64_t, Cargo*> map_of_curgo_;
};

class OrderDB;

class Company : public QObject
{
    Q_OBJECT

public:
    explicit Company(QObject* parent = nullptr);
    ~Company() override;

    void setName(const QString& name);
    void addWarehouse(Warehouse* warehouse);
    void addDestination(company_item::Destination* destination);

    const QString getName() const;

    void restorCompany(const OrderDB *route_db);

signals:
    void addedNewWarehouse(Warehouse*);
    void addedNewDestination(company_item::Destination*);

private:
    QString               company_name_;
    QVector<Warehouse*>   warehouses_;
    QVector<company_item::Destination*> destinations_;
};

#endif // COMPANY_H
