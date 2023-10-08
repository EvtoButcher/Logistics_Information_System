#ifndef ROUTEDB_H
#define ROUTEDB_H

#include <QSqlDatabase>
#include <QSqlQuery>

class RouteDB
{
public:
    RouteDB();
    ~RouteDB();

    const QSqlDatabase &DB();

private:
    QSqlDatabase db;
    QSqlQuery*   query;
};

#endif // ROUTEDB_H
