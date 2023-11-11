#include <QDebug>
#include <QFileInfo>
#include <QLocale>
#include <QDate>
#include <QSqlError>
#include <QSql>
#include <QSqlQuery>
#include <QFileDialog>
#include <QDateTime>

#include "OrderDB.h"
#include "common.h"
#include "ApplicationSettings.h"

OrderDB::OrderDB(const QString& name, QObject *parent)
     : QObject(parent)
     , db(QSqlDatabase::addDatabase("QSQLITE"))
{
    query = new QSqlQuery(db);

    if(common::fileExists(name)) {
        openDB(name);
    }
    else {
        createDB(name + ".db");
    }
}

const QSqlDatabase &OrderDB::DB() const
{
    return db;
}

void OrderDB::inserIntoOrderTable(const RouteInfo &info)
{
    query->prepare("INSERT INTO " MAIN_TABLE "("
                                              "color, "
                                              "start_p, "
                                              "end_p, "
                                              "start_time, "
                                              "distance, "
                                              "end_time, "
                                              "code) "
                  "VALUES (:RouteColor, :StartPos, :EndPos, :StartDate, :Distance, :EndDate, :Code)");

    const double tmp_s_lat = info.start_route_point_.latitude();
    const double tmp_s_lng = info.start_route_point_.longitude();
    const QString start = QString::number(tmp_s_lat, 'f', 6) + " " + QString::number(tmp_s_lng,'f', 6);

    const double tmp_e_lat = info.end_route_point_.latitude();
    const double tmp_e_lng = info.end_route_point_.longitude();
    const QString end = QString::number(tmp_e_lat, 'f', 6) + " " + QString::number(tmp_e_lng, 'f', 6);

    query->bindValue(":Code",       info.code_);
    query->bindValue(":StartPos",   start);
    query->bindValue(":EndPos",     end);
    query->bindValue(":Distance",   000);
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    query->bindValue(":StartDate",  QDateTime::currentDateTime().toString(Qt::DateFormat::LocalDate));
    query->bindValue(":EndDate",    QDateTime::currentDateTime().toString(Qt::DateFormat::LocalDate));
    #pragma GCC diagnostic pop

    query->bindValue(":RouteColor", info.route_color_);

    if(!query->exec()) {
        qDebug() << "error insert into " << MAIN_TABLE;
        qDebug() << query->lastError().text();
     }
     else {
        qDebug() << query->lastQuery();
    }
}

void OrderDB::updateDistanceFromOrderTable(const QString &code, const int distance)
{
    query->prepare("UPDATE " MAIN_TABLE " SET distance = :Distance WHERE code = :Code");
    query->bindValue(":Distance", distance);
    query->bindValue(":Code", code);

    if(!query->exec()) {
        qDebug() << "error updateDistance into " << MAIN_TABLE;
        qDebug() << query->lastError().text();
     }
     else {
        qDebug() << query->lastQuery();
    }
}

void OrderDB::deleteFromOrderTable(const int index)
{
    deleteFromPathTable(index);

    query->prepare("DELETE FROM " MAIN_TABLE " WHERE id = :Index");
    query->bindValue(":Index", index);

    if(!query->exec()) {
        qDebug() << "error delete " << MAIN_TABLE;
        qDebug() << query->lastError().text();
     }
     else {
        qDebug() << "acept delete";
    }
}

void OrderDB::updateColor(const int index, const QString &color)
{
    query->prepare("UPDATE " MAIN_TABLE " SET color = :Color WHERE id = :Id");
    query->bindValue(":Id", index);
    query->bindValue(":Color", color);

    if(!query->exec()) {
        qDebug() << "error update Color into " << MAIN_TABLE;
        qDebug() << query->lastError().text();
     }
     else {
        qDebug() << query->lastQuery();
    }
}

QColor OrderDB::selectColor(const int index)
{
    query->prepare("SELECT color FROM " MAIN_TABLE " WHERE id = :Id");
    query->bindValue(":Id", index);

    if (query->exec() && query->next()) {
     /* qDebug() << query->value(0).toString();*///      path = query.value(0).toString();
    }

    return QColor(query->value(0).toString());
}

void OrderDB::insrtrIntoPathTable(const QString& main_code, const QVector<QGeoCoordinate>& position_caсhe)
{
    QString path;

    for(const auto& coordinate : position_caсhe) {
        path += QString::number(coordinate.latitude(), 'f', 6) + " " + QString::number(coordinate.longitude(), 'f', 6) + " ";
    }

    query->prepare("INSERT INTO " PATH_TABLE " ("         //
                                           "main_code, "  //TODO: add error handling
                                           "path) "       //
                   "VALUES (:MainCode,  :Path)");

    query->bindValue(":MainCode", main_code);
    query->bindValue(":Path", path);


    if(!query->exec()) {
        qDebug() << "error insert into " << PATH_TABLE;
        qDebug() << query->lastError().text();
     }
     else {
        qDebug() << query->lastQuery();
     }
}

const QString OrderDB::selectPath(const QString code)
{
    query->prepare("SELECT path FROM " PATH_TABLE " WHERE main_code = :Code");
    query->bindValue(":Code", code);

    if (query->exec() && query->next()) {
     /* qDebug() << query->value(0).toString();*///      path = query.value(0).toString();
    }

    return query->value(0).toString();
}

void OrderDB::insertIntoWarehouseTable(const uint64_t code, const QGeoCoordinate position)
{

    query->prepare("INSERT INTO " WAREHOUSE_TABLE " ("          //
                                                  "code, "      //TODO: add error handling
                                                  "position) "  //
                   "VALUES (:Code,  :Pos)");

    qDebug() << QString::number(code) << QString::number(position.latitude(), 'f', 6) + " " + QString::number(position.longitude(), 'f', 6);
    query->bindValue(":Code", QString::number(code));
    query->bindValue(":Pos",  QString::number(position.latitude(), 'f', 6) + " " + QString::number(position.longitude(), 'f', 6));


    if(!query->exec()) {
        qDebug() << "error insert into " << WAREHOUSE_TABLE;
        qDebug() << query->lastError().text();
     }
     else {
        qDebug() << query->lastQuery();
    }
}

void OrderDB::insertIntoDestinationTable(const uint64_t code, const QGeoCoordinate position)
{
    query->prepare("INSERT INTO " DESTINATION_TABLE " ("        //
                                                  "code, "      //TODO: add error handling
                                                  "position) "  //
                   "VALUES (:Code,  :Pos)");

    qDebug() << QString::number(code) << QString::number(position.latitude(), 'f', 6) + " " + QString::number(position.longitude(), 'f', 6);
    query->bindValue(":Code", QString::number(code));
    query->bindValue(":Pos",  QString::number(position.latitude(), 'f', 6) + " " + QString::number(position.longitude(), 'f', 6));


    if(!query->exec()) {
        qDebug() << "error insert into " << DESTINATION_TABLE;
        qDebug() << query->lastError().text();
     }
     else {
        qDebug() << query->lastQuery();
    }
}

std::optional<QSqlError> OrderDB::createTables()
{

    query->prepare("CREATE TABLE " MAIN_TABLE " ("
                                                "id INTEGER PRIMARY KEY, "
                                                "color      CHAR, "
                                                "start_p    CHAR, "
                                                "end_p      CHAR, "
                                                "start_time TEXT, "
                                                "distance   INTEGER, "
                                                "end_time   TEXT, "
                                                "code       CHAR)");
    if(!query->exec()){
        return std::make_optional(query->lastError());
    }

    query->prepare("CREATE TABLE " PATH_TABLE " ("
                                                "id         INTEGER PRIMARY KEY, "
                                                "main_code  CHAR, "
                                                "path       TEXT, "
                                                "FOREIGN KEY (main_code) REFERENCES " MAIN_TABLE "(code))");

    if(!query->exec()){
         return std::make_optional(query->lastError());
    }

    query->prepare("CREATE TABLE " WAREHOUSE_TABLE " ("
                                                   "id         INTEGER PRIMARY KEY, "
                                                   "code       CHAR, "
                                                   "position   CHAR )");

    if(!query->exec()){
         return std::make_optional(query->lastError());
    }

    query->prepare("CREATE TABLE " DESTINATION_TABLE " ("
                                                   "id         INTEGER PRIMARY KEY, "
                                                   "code       CHAR, "
                                                   "position   CHAR )");

    if(!query->exec()){
         return std::make_optional(query->lastError());
    }

    return std::nullopt;
}

bool OrderDB::openDB(const QString& db_name)
{
    db.setDatabaseName(db_name);
    if(db.open()){
        emit DbIsOpen();
        return true;
    }
    return false;
}

bool OrderDB::closeDB()
{
    if(!db.isOpen()){
        return false;
    }
    db.close();
    emit DbIsClose();
    return true;
}

bool OrderDB::importDB()
{
   QFile db_file = QFileDialog::getOpenFileName(nullptr ,tr("Open Database"), QDir::currentPath(),tr("*.db"));

   return openDB(db_file.fileName());
}

void OrderDB::createDB(const QString& db_name)
{
    db.setDatabaseName(db_name);

    if(db.open()){
        if(createTables()){
            //TODO: add create_tables_error handling
        }
    }
    else {
      //TODO: add open_db_error handling
    }
}

void OrderDB::deleteFromPathTable(const int index)
{
    query->prepare("DELETE FROM " PATH_TABLE " WHERE id = :Index");
    query->bindValue(":Index", index);

    if(!query->exec()) {
        qDebug() << "error delete " << PATH_TABLE;
        qDebug() << query->lastError().text();
     }
     else {
        qDebug() << "acept delete";
     }
}
