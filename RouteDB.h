#ifndef ROUTEDB_H
#define ROUTEDB_H

#include <QObject>
#include <QSqlDatabase>

#include "RouteModel.h"

class QSqlQuery;

#define DATABASE_NAME       "routesDB.db"
#define MAIN_TABLE          "Routes"

class RouteDB : public QObject
{
    Q_OBJECT

public:
    RouteDB(QObject *parent = nullptr);
    ~RouteDB() = default;

    const QSqlDatabase &DB() const;

    bool inserIntoTable(const RouteInfo info);
    bool deleteFromTable(const int index);

    bool closeDB();
    bool importDB();

private:
    bool createTable();
    bool openDB();
    bool restoreDB();

private:
    QSqlDatabase db;
    QSqlQuery* query;
};

#endif // ROUTEDB_H
