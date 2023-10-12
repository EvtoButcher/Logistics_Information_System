#ifndef ROUTEDB_H
#define ROUTEDB_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

#include "RouteModel.h"

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

private:
    bool createTable();
    bool openDB();
    bool restoreDB();

private:
    QSqlDatabase db;
};

#endif // ROUTEDB_H
