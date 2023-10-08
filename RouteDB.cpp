#include "RouteDB.h"
#include "common.h"

#include <QDebug>
#include <QFileInfo>

RouteDB::RouteDB()
    : db(QSqlDatabase::addDatabase("QSQLITE"))
{
    db.setDatabaseName("./routesDB.db");

    if(db.open()){
        qDebug() << "open";
    }else{
        qDebug() << "error";
    }

    query = new QSqlQuery(db);

    if(!fileExists("./routesDB.db")){
        query->exec("CREATE TABLE "
                    "RoutesStartPosition ("
                                        "id int AUTO_INCREMENT PRIMARY KEY, "
                                        "s_lat DOUBLE, "
                                        "s_lng DOUBLE"
                    "                   );");

        query->exec("CREATE TABLE "
                    "RoutesEndPosition ("
                                    "id int AUTO_INCREMENT PRIMARY KEY, "
                                    "e_lat DOUBLE, "
                                    "e_lng DOUBLE"
                                    ");");

        query->exec("CREATE TABLE "
                    "Routes ("
                    "id int AUTO_INCREMENT PRIMARY KEY, "
                    "name char, "
                    "start_p POINT, "
                    "end_p POINT, "
                    "time DateTime, "
                    "FOREIGN KEY (start_p) REFERENCES RoutesStartPosition(id), "
                    "FOREIGN KEY (end_p) REFERENCES RoutesEndPosition(id));");
    }
}

RouteDB::~RouteDB()
{
    delete query;
}

const QSqlDatabase &RouteDB::DB()
{
    return db;
}
