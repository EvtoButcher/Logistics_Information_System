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

    //if(!fileExists("./routesDB.db")){
        query->exec("CREATE TABLE "
                    "RoutesStartPosition ("
                                        "id int, "
                                        "s_lat DOUBLE, "
                                        "s_lng DOUBLE, "
                                        "PRIMARY KEY (id));");

        query->exec("CREATE TABLE "
                    "RoutesEndPosition ("
                                    "id int, "
                                    "e_lat DOUBLE, "
                                    "e_lng DOUBLE, "
                                    "PRIMARY KEY (id));");

        query->exec("CREATE TABLE "
                    "Routes ("
                    "id int, "
                    "name char, "
                    "start_p POINT, "
                    "end_p POINT, "
                    "time DateTime, "
                    "PRIMARY KEY (id), "
                    "FOREIGN KEY (start_p) REFERENCES RoutesStartPosition(id), "
                    "FOREIGN KEY (end_p) REFERENCES RoutesEndPosition(id));");
    //}


}

RouteDB::~RouteDB()
{
    delete query;
}

const QSqlDatabase &RouteDB::DB()
{
    return db;
}
