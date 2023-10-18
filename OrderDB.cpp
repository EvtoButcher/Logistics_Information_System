#include "OrderDB.h"

#include <QDebug>
#include <QFileInfo>
#include <QLocale>
#include <QDate>
#include <QSqlError>
#include <QSql>
#include <QSqlQuery>
#include <QFileDialog>
#include <QDateTime>

#include "common.h"

OrderDB::OrderDB(QObject *parent)
     : QObject(parent)
     , db(QSqlDatabase::addDatabase("QSQLITE"))
{
    query = new QSqlQuery(db);

    if(fileExists(DATABASE_NAME)){
        openDB(DATABASE_NAME);
        qDebug() << "open";
    }
    else{
        restoreDB();
        qDebug() << "restore";
    }
}

const QSqlDatabase &OrderDB::DB() const
{
    return db;
}

bool OrderDB::inserIntoOrderTable(const RouteInfo &info)
{
    query->prepare("INSERT INTO " MAIN_TABLE "("
                                              "color, "
                                              "start_p, "
                                              "end_p, "
                                              "start_time, "
                                              "end_time, "
                                              "code) "
                  "VALUES (:RouteColor, :StartPos, :EndPos, :StartDate, :EndDate, :Code)");

    double tmp_s_lat = info.start_route_point_.latitude();
    double tmp_s_lng = info.start_route_point_.longitude();
    QString start = QString::number(tmp_s_lat, 'f', 6) + " " + QString::number(tmp_s_lng,'f', 6);

    double tmp_e_lat = info.end_route_point_.latitude();
    double tmp_e_lng = info.end_route_point_.longitude();
    QString end = QString::number(tmp_e_lat, 'f', 6) + " " + QString::number(tmp_e_lng, 'f', 6);

    query->bindValue(":Code",       info.code_);
    query->bindValue(":StartPos",   start);
    query->bindValue(":EndPos",     end);
    query->bindValue(":StartDate",  QDateTime::currentDateTime().toString(Qt::DateFormat::LocalDate));
    query->bindValue(":EndDate",    QDateTime::currentDateTime().toString(Qt::DateFormat::LocalDate));
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

bool OrderDB::deleteFromOrderTable(const int index)
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

bool OrderDB::insrtrIntoPathTable(const QString& main_code, const QVector<QGeoCoordinate>& position_caсhe)
{
    QString path;

    for(const auto& coordinate : position_caсhe){
        path += QString::number(coordinate.latitude(), 'f', 6) + " " + QString::number(coordinate.longitude(), 'f', 6) + " ";
    }

    qDebug() << position_caсhe.count();
    query->prepare("INSERT INTO " PATH_TABLE " ("         //
                                           "main_code, "  //TODO: add error handling
                                           "path) "       //
                   "VALUES (:MainCode,  :Path)");

    query->bindValue(":MainCode", main_code);
    query->bindValue(":Path", path);


    if(!query->exec()){
        qDebug() << "error insert into " << PATH_TABLE;
        qDebug() << query->lastError().text();
        return false;
     }
     else{
        qDebug() << query->lastQuery();
        return true;
     }
    return false;
}

const QString OrderDB::selectPath(QString code)
{
    query->prepare("SELECT path FROM " PATH_TABLE " WHERE main_code = :Code");
    query->bindValue(":Code", code);

    query->exec();

    if (query->exec() && query->next()) {
     /* qDebug() << query->value(0).toString();*///      path = query.value(0).toString();
    }

    return query->value(0).toString();
}

//bool OrderDB::deleteFromPathTable(const int index)
//{

//}

bool OrderDB::createTable()
{

    if(!query->exec("CREATE TABLE " MAIN_TABLE " ("
                                                "id INTEGER PRIMARY KEY, "
                                                "color      CHAR, "
                                                "start_p    CHAR, "
                                                "end_p      CHAR, "
                                                "start_time TEXT, "
                                                "end_time   TEXT, "
                                                "code       CHAR)")){
        qDebug() << "DataBase: error of create " << MAIN_TABLE;
        qDebug() << query->lastError().text();
        return false;
    }

    if(!query->exec("CREATE TABLE " PATH_TABLE " ("
                                                "id         INTEGER PRIMARY KEY, "
                                                "main_code  CHAR, "
                                                "path       TEXT, "
                                                "FOREIGN KEY (main_code) REFERENCES " MAIN_TABLE "(code))")){
        qDebug() << "DataBase: error of create " << PATH_TABLE;
        qDebug() << query->lastError().text();
        return false;
    }

    return true;
}

bool OrderDB::openDB(QString db_name)
{
    db.setDatabaseName(db_name);
    return db.open();
}

bool OrderDB::closeDB()
{
    if(!db.isOpen()){
        return false;
    }
    db.close();
    return true;
}

bool OrderDB::importDB()
{
   QFile db_file = QFileDialog::getOpenFileName(nullptr ,tr("Open Database"), QDir::currentPath(),tr("*.db"));

   return openDB(db_file.fileName());
}

bool OrderDB::restoreDB()
{
    if(openDB(DATABASE_NAME)){
        return !createTable();
    }
    else{
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    return false;
}
