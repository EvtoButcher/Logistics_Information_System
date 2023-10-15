#include "RouteDB.h"

#include <QDebug>
#include <QFileInfo>
#include <QLocale>
#include <QDate>
#include <QSqlError>
#include <QSql>
#include <QSqlQuery>

#include "common.h"

RouteDB::RouteDB(QObject *parent)
     : QObject(parent)
     , db(QSqlDatabase::addDatabase("QSQLITE"))
{
    query = new QSqlQuery(db);

    if(fileExists("./" DATABASE_NAME)){
        openDB();           //TODO: incorrect display of routes during
                            //      deserialization from the database.
                            //      it doesn't make sense at the moment
        qDebug() << "open";
    }
    else{
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
    query->prepare("INSERT INTO " MAIN_TABLE "("
                                              "name, "
                                              "start_p, "
                                              "end_p, "
                                              "time, "
                                              "color) "
                  "VALUES (:Name, :StartPos, :EndPos, :DATE, :RouteColor)");

    double tmp_s_lat = info.start_route_point_.first;
    double tmp_s_lng = info.start_route_point_.second;
    QString start = QString::number(tmp_s_lat, 'f', 6) + " " + QString::number(tmp_s_lng,'f', 6);

    double tmp_e_lat = info.end_route_point_.first;
    double tmp_e_lng = info.end_route_point_.second;
    QString end = QString::number(tmp_e_lat, 'f', 6) + " " + QString::number(tmp_e_lng, 'f', 6);

    query->bindValue(":Name",       info.name_);
    query->bindValue(":StartPos",   start);
    query->bindValue(":EndPos",     end);
    query->bindValue(":DATE",       "");
    query->bindValue(":RouteColor", info.route_color_);

    if(!query->exec()){
        qDebug() << "error insert into " << MAIN_TABLE;
        qDebug() << query->lastError().text();
        return false;
     }
     else{
        qDebug() << query->lastQuery();
        return true;
     }
    return false;
}

bool RouteDB::deleteFromTable(const int index)
{
    query->prepare("DELETE FROM " MAIN_TABLE " WHERE id = :Index");
    query->bindValue(":Index", index);

    if(!query->exec()){
        qDebug() << "error delete " << MAIN_TABLE;
        qDebug() << query->lastError().text();
        return false;
     }
     else{
        qDebug() << "acept delete";
        return true;
     }
    return false;
}

bool RouteDB::createTable()
{

    if(!query->exec("CREATE TABLE " MAIN_TABLE "("
                                                "id INTEGER PRIMARY KEY, "
                                                "name char, "
                                                "start_p char, "
                                                "end_p char, "
                                                "time DateTime, "
                                                "color char)")){
        qDebug() << "DataBase: error of create " << MAIN_TABLE;
        qDebug() << query->lastError().text();
        return false;
    }
    else {
        return true;
    }
    return false;
}

bool RouteDB::openDB()
{
    db.setDatabaseName("./" DATABASE_NAME);
    return db.open();
}

bool RouteDB::closeDB()
{
    if(!db.isOpen()){
        return false;
    }
    db.close();
    return true;
}

bool RouteDB::restoreDB()
{
    if(openDB()){
        return !createTable();
    }
    else{
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    return false;
}
