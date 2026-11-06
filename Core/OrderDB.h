#ifndef ORDERDB_H
#define ORDERDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QColor>
#include <QVector>
#include <QGeoCoordinate>
#include <QString>
#include <cstdint>


#define MAIN_TABLE         "Orders"
#define PATH_TABLE         "PathTable"
#define WAREHOUSE_TABLE    "Warehouse_table"
#define DESTINATION_TABLE  "Destination_table"
#define CARGO_TABLE        "Cargo_table"

class RouteInfo;
class QSqlQuery;

struct CargoRecord
{
    uint64_t code_ = 0;
    double   weight_ = 0.0;
    int      volume_ = 0;
};

class OrderDB : public QObject
{
    Q_OBJECT

public:
    explicit OrderDB(const QString& name, QObject* parent = nullptr);
    ~OrderDB() override;

    const QSqlDatabase& DB() const;
    bool                isOpen() const;
    QString             lastError() const;

    QColor                 selectColor(const int index) const;
    QString                selectPath(const QString& code) const;
    QVector<CargoRecord>   selectCargo(const QString& warehouseCode) const;

    void deleteFromOrderTable(const int index);
    void updateColor(const int index, const QString& color);
    void updateDistanceFromOrderTable(const QString& code, const double distance);

    void insertIntoOrderTable(const RouteInfo& info);
    void insertIntoWarehouseTable(const uint64_t code, const QGeoCoordinate& position);
    void insertIntoDestinationTable(const uint64_t code, const QGeoCoordinate& position);
    void insertIntoPathTable(const QString& main_code, const QVector<QGeoCoordinate>& position_cache);
    void insertCargo(const QString& warehouseCode, const uint64_t code, const double weight, const int volume);
    void deleteWarehouse(const uint64_t code);
    void deleteDestination(const uint64_t code);

    bool closeDB();
    bool importDB();

signals:
    void DbIsOpen();
    void DbIsClose();

private:
    bool initializeSchema();
    bool createTables();
    bool migrateLegacySchema();
    bool tableExists(const QString& table) const;
    int  schemaVersion() const;
    bool setSchemaVersion(const int version);

    bool exec(QSqlQuery& query, const QString& sql);
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

    QString              serializePath(const QVector<QGeoCoordinate>& path) const;
    QVector<QGeoCoordinate> deserializePath(const QString& text) const;
    bool                 backupDatabase(const QString& path);
    void                 setLastError(const QString& error);

private:
    QSqlDatabase db;
    QString      connectionName_;
    QString      dbPath_;
    QString      lastError_;
};

#endif // ORDERDB_H
