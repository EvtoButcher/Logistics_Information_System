#ifndef ORDERDB_H
#define ORDERDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <optional>

#include "RouteModel.h"

class QSqlQuery;

#define DATABASE_NAME       "orderDB.db"//
#define MAIN_TABLE          "Orders"    //TODO: replace with file.conf
#define PATH_TABLE          "PathTable" //

class OrderDB : public QObject
{
    Q_OBJECT

public:
    OrderDB(QObject *parent = nullptr);
    ~OrderDB() = default;

    const QSqlDatabase& DB() const;

    void inserIntoOrderTable(const RouteInfo& info);
    void updateDistanceFromOrderTable(const QString& code, const int distance);
    void deleteFromOrderTable(const int index);

    bool closeDB();
    bool importDB();

    void insrtrIntoPathTable(const QString& main_code, const QVector<QGeoCoordinate>& position_cahe);

    const QString selectPath(const QString code);

private:
    std::optional<QSqlError> createTables();
    bool openDB(QString db_name);
    void createDB();

    void deleteFromPathTable(const int index);

private:
    QSqlDatabase db;
    QSqlQuery*   query;
};

#endif // ORDERDB_H
