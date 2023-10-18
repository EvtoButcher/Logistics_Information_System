#ifndef ORDERDB_H
#define ORDERDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QString>

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

    bool inserIntoOrderTable(const RouteInfo& info);
    bool deleteFromOrderTable(const int index);

    bool closeDB();
    bool importDB();

    bool insrtrIntoPathTable(const QString& main_code, const QVector<QGeoCoordinate>& position_cahe);
    //bool deleteFromPathTable(const int index);

    const QString selectPath(QString code);

private:
    bool createTable();
    bool openDB(QString db_name);
    bool restoreDB();

private:
    QSqlDatabase db;
    QSqlQuery*   query;
};

#endif // ORDERDB_H
