#include "RouteDB.h"
#include "common.h"

#include <QDebug>
#include <QFileInfo>

RouteDB::RouteDB(QObject *parent)
     : QObject(parent)
     , db(QSqlDatabase::addDatabase("QSQLITE"))
{

    if(fileExists("./" DATABASE_NAME)){
        openDB();
        qDebug() << "open";
    }else{
        restoreDB();
        qDebug() << "restore";
    }
}


const QSqlDatabase &RouteDB::DB() const
{
    return db;
}

bool RouteDB::inserIntoTable(const RouteInfo info)
{
    QSqlQuery query;

    query.prepare("INSERT INTO " MAIN_TABLE "(name,"
                                         " start_p,"
                                         " end_p,"
                                         " time) "
               "VALUES (:Name, :StartPos, :EndPos, :DATE )");

    double tmp_s_lat = info.start_route_point_.first;
    double tmp_s_lng = info.start_route_point_.second;
    QString start = QString::number(tmp_s_lat) + " " + QString::number(tmp_s_lng);

    double tmp_e_lat = info.end_route_point_.first;
    double tmp_e_lng = info.end_route_point_.second;
    QString end = QString::number(tmp_e_lat) + " " + QString::number(tmp_e_lng);

    query.bindValue(":Name",     info.name_);
    query.bindValue(":StartPos", start);
    query.bindValue(":EndPos", end);
    query.bindValue(":DATE", "");

//                "VALUES ('" + info.name_ + "', "
//                "POINT(" + info.start_route_point_.first + "," + info.start_route_point_.second + "), "
//                    "POINT(" + info.end_route_point_.first + "," +  info.end_route_point_.second + "), '2021-01-01 10:00:00');");
    if(!query.exec()){
            qDebug() << "error insert into " << MAIN_TABLE;
            qDebug() << query.lastError().text();
            return false;
        } else {
            return true;
        }
        return false;
}

bool RouteDB::createTable()
{
    QSqlQuery query(db);

//    query.exec("CREATE TABLE "
//                        "RoutesStartPosition ("
//                                            "id int AUTO_INCREMENT PRIMARY KEY, "
//                                            "s_lat DOUBLE, "
//                                            "s_lng DOUBLE"
//                                            ");");

//    query.exec("CREATE TABLE "
//                "RoutesEndPosition ("
//                                "id int AUTO_INCREMENT PRIMARY KEY, "
//                                "e_lat DOUBLE, "
//                                "e_lng DOUBLE"
//                                ");");

    if(!query.exec("CREATE TABLE "
                     MAIN_TABLE "("
                            "id int AUTO_INCREMENT PRIMARY KEY, "
                            "name char, "
                            "start_p char, "
                            "end_p char, "
                            "time DateTime);"
//                            "FOREIGN KEY (start_p) REFERENCES RoutesStartPosition(id), "
                            //"FOREIGN KEY (end_p) REFERENCES RoutesEndPosition(id));"
                        )){
            qDebug() << "DataBase: error of create " << MAIN_TABLE;
            qDebug() << query.lastError().text();
            return false;
        } else {
            return true;
        }
        return false;
}

bool RouteDB::openDB()
{
    db.setDatabaseName("./" DATABASE_NAME);

    return db.open();
}

bool RouteDB::restoreDB()
{
    if(openDB()){
           if(!this->createTable()){
               return false;
           } else {
               return true;
           }
       } else {
           qDebug() << "Не удалось восстановить базу данных";
           return false;
       }
       return false;
}
