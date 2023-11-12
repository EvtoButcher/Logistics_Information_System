#ifndef ORDERDB_H
#define ORDERDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QColor>
#include <optional>
#include <QGeoCoordinate>


#define MAIN_TABLE         "Orders"    //TODO: replace with file.conf
#define PATH_TABLE         "PathTable" //
#define WAREHOUSE_TABLE    "Warehouse_table"
#define DESTINATION_TABLE  "Destination_table"


class RouteInfo;
class QSqlQuery;
class QString;

enum ConnectionStatus{
    Offline,
    Online
};

class OrderDB : public QObject
{
    Q_OBJECT

public:
    explicit OrderDB(const QString& name, QObject* parent = nullptr);
    ~OrderDB() = default;

    const QSqlDatabase& DB() const;

    QColor  selectColor(const int index)    const;
    QString selectPath(const QString& code) const;

    void deleteFromOrderTable(const int index);
    void updateColor(const int index, const QString& color);
    void updateDistanceFromOrderTable(const QString& code, const int distance);

    void inserIntoOrderTable(const RouteInfo& info);
    void insertIntoWarehouseTable(const uint64_t code, const QGeoCoordinate position);
    void insertIntoDestinationTable(const uint64_t code, const QGeoCoordinate position);
    void insrtrIntoPathTable(const QString& main_code, const QVector<QGeoCoordinate>& position_cahe);

    bool closeDB();
    bool importDB();

signals:
    void DbIsOpen();
    void DbIsClose();

private:
    std::optional<QSqlError> createTables();
    bool openDB(const QString& db_name);
    void createDB(const QString& db_name);

    void deleteFromPathTable(const int index);

private:
    QSqlDatabase db;
    QSqlQuery*   query;
};

#endif // ORDERDB_H
