#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QSqlError>
#include <QSqlQuery>

#include "OrderDB.h"
#include "ApplicationSettings.h"
#include "ModelForQML/RouteModel.h"

namespace {

QString positionToString(const QGeoCoordinate& position)
{
    return QString::number(position.latitude(), 'f', 6) + " " +
           QString::number(position.longitude(), 'f', 6);
}

} // namespace

OrderDB::OrderDB(const QString& name, QObject* parent)
    : QObject(parent)
    , connectionName_(QStringLiteral("orders_") + QString::number(reinterpret_cast<quintptr>(this), 16))
{
    dbPath_ = name.isEmpty() ? ApplicationSettings::dbPath() : name;

    db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), connectionName_);
    db.setDatabaseName(dbPath_);

    if(!db.open()) {
        setLastError(db.lastError().text());
        return;
    }

    QSqlQuery pragma(db);
    exec(pragma, QStringLiteral("PRAGMA foreign_keys = ON"));
    exec(pragma, QStringLiteral("PRAGMA busy_timeout = 5000"));

    if(!initializeSchema()) {
        db.close();
        return;
    }

    emit DbIsOpen();
}

OrderDB::~OrderDB()
{
    if(db.isValid()) {
        db.close();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(connectionName_);
    }
}

const QSqlDatabase& OrderDB::DB() const
{
    return db;
}

bool OrderDB::isOpen() const
{
    return db.isOpen();
}

QString OrderDB::lastError() const
{
    return lastError_;
}

QColor OrderDB::selectColor(const int index) const
{
    QSqlQuery query(db);
    query.prepare(QStringLiteral("SELECT color FROM ") + MAIN_TABLE + QStringLiteral(" WHERE id = :Id"));
    query.bindValue(QStringLiteral(":Id"), index);

    if(!query.exec() || !query.next()) {
        return QColor();
    }

    return QColor(query.value(0).toString());
}

QString OrderDB::selectPath(const QString& code) const
{
    QSqlQuery query(db);
    query.prepare(QStringLiteral("SELECT path FROM ") + PATH_TABLE + QStringLiteral(" WHERE main_code = :Code"));
    query.bindValue(QStringLiteral(":Code"), code);

    if(!query.exec() || !query.next()) {
        return QString();
    }

    return query.value(0).toString();
}

QVector<CargoRecord> OrderDB::selectCargo(const QString& warehouseCode) const
{
    QVector<CargoRecord> cargo;

    QSqlQuery query(db);
    query.prepare(QStringLiteral("SELECT code, weight, volume FROM ") + CARGO_TABLE +
                  QStringLiteral(" WHERE warehouse_code = :Code"));
    query.bindValue(QStringLiteral(":Code"), warehouseCode);

    if(!query.exec()) {
        return cargo;
    }

    while(query.next()) {
        CargoRecord record;
        record.code_ = query.value(0).toULongLong();
        record.weight_ = query.value(1).toDouble();
        record.volume_ = query.value(2).toInt();
        cargo.push_back(record);
    }

    return cargo;
}

void OrderDB::deleteFromOrderTable(const int index)
{
    QSqlQuery select(db);
    select.prepare(QStringLiteral("SELECT code FROM ") + MAIN_TABLE + QStringLiteral(" WHERE id = :Id"));
    select.bindValue(QStringLiteral(":Id"), index);

    QString code;
    if(select.exec() && select.next()) {
        code = select.value(0).toString();
    }

    if(!beginTransaction()) {
        return;
    }

    if(!code.isEmpty()) {
        QSqlQuery deletePath(db);
        deletePath.prepare(QStringLiteral("DELETE FROM ") + PATH_TABLE + QStringLiteral(" WHERE main_code = :Code"));
        deletePath.bindValue(QStringLiteral(":Code"), code);
        if(!deletePath.exec()) {
            setLastError(deletePath.lastError().text());
            rollbackTransaction();
            return;
        }
    }

    QSqlQuery deleteOrder(db);
    deleteOrder.prepare(QStringLiteral("DELETE FROM ") + MAIN_TABLE + QStringLiteral(" WHERE id = :Id"));
    deleteOrder.bindValue(QStringLiteral(":Id"), index);
    if(!deleteOrder.exec()) {
        setLastError(deleteOrder.lastError().text());
        rollbackTransaction();
        return;
    }

    commitTransaction();
}

void OrderDB::updateColor(const int index, const QString& color)
{
    QSqlQuery query(db);
    query.prepare(QStringLiteral("UPDATE ") + MAIN_TABLE +
                  QStringLiteral(" SET color = :Color WHERE id = :Id"));
    query.bindValue(QStringLiteral(":Color"), color);
    query.bindValue(QStringLiteral(":Id"), index);

    if(!query.exec()) {
        setLastError(query.lastError().text());
    }
}

void OrderDB::updateDistanceFromOrderTable(const QString& code, const double distance)
{
    QSqlQuery query(db);
    query.prepare(QStringLiteral("UPDATE ") + MAIN_TABLE +
                  QStringLiteral(" SET distance = :Distance WHERE code = :Code"));
    query.bindValue(QStringLiteral(":Distance"), distance);
    query.bindValue(QStringLiteral(":Code"), code);

    if(!query.exec()) {
        setLastError(query.lastError().text());
    }
}

void OrderDB::insertIntoOrderTable(const RouteInfo& info)
{
    QSqlQuery query(db);
    query.prepare(QStringLiteral("INSERT INTO ") + MAIN_TABLE +
                  QStringLiteral(" (color, start_p, end_p, start_time, distance, end_time, code) "
                                 "VALUES (:Color, :StartPos, :EndPos, :StartTime, :Distance, :EndTime, :Code)"));

    query.bindValue(QStringLiteral(":Color"), info.route_color_);
    query.bindValue(QStringLiteral(":StartPos"), positionToString(info.start_route_point_));
    query.bindValue(QStringLiteral(":EndPos"), positionToString(info.end_route_point_));
    query.bindValue(QStringLiteral(":StartTime"), QDateTime::currentDateTime().toString(Qt::ISODate));
    query.bindValue(QStringLiteral(":Distance"), 0.0);
    query.bindValue(QStringLiteral(":EndTime"), QDateTime::currentDateTime().toString(Qt::ISODate));
    query.bindValue(QStringLiteral(":Code"), info.code_);

    if(!query.exec()) {
        setLastError(query.lastError().text());
    }
}

void OrderDB::insertIntoWarehouseTable(const uint64_t code, const QGeoCoordinate& position)
{
    QSqlQuery query(db);
    query.prepare(QStringLiteral("INSERT INTO ") + WAREHOUSE_TABLE +
                  QStringLiteral(" (code, position) VALUES (:Code, :Position)"));
    query.bindValue(QStringLiteral(":Code"), QString::number(code));
    query.bindValue(QStringLiteral(":Position"), positionToString(position));

    if(!query.exec()) {
        setLastError(query.lastError().text());
    }
}

void OrderDB::insertIntoDestinationTable(const uint64_t code, const QGeoCoordinate& position)
{
    QSqlQuery query(db);
    query.prepare(QStringLiteral("INSERT INTO ") + DESTINATION_TABLE +
                  QStringLiteral(" (code, position) VALUES (:Code, :Position)"));
    query.bindValue(QStringLiteral(":Code"), QString::number(code));
    query.bindValue(QStringLiteral(":Position"), positionToString(position));

    if(!query.exec()) {
        setLastError(query.lastError().text());
    }
}

void OrderDB::insertIntoPathTable(const QString& main_code, const QVector<QGeoCoordinate>& position_cache)
{
    QSqlQuery deleteQuery(db);
    deleteQuery.prepare(QStringLiteral("DELETE FROM ") + PATH_TABLE + QStringLiteral(" WHERE main_code = :Code"));
    deleteQuery.bindValue(QStringLiteral(":Code"), main_code);
    if(!deleteQuery.exec()) {
        setLastError(deleteQuery.lastError().text());
        return;
    }

    QSqlQuery query(db);
    query.prepare(QStringLiteral("INSERT INTO ") + PATH_TABLE +
                  QStringLiteral(" (main_code, path) VALUES (:Code, :Path)"));
    query.bindValue(QStringLiteral(":Code"), main_code);
    query.bindValue(QStringLiteral(":Path"), serializePath(position_cache));

    if(!query.exec()) {
        setLastError(query.lastError().text());
    }
}

void OrderDB::insertCargo(const QString& warehouseCode, const uint64_t code, const double weight, const int volume)
{
    QSqlQuery query(db);
    query.prepare(QStringLiteral("INSERT INTO ") + CARGO_TABLE +
                  QStringLiteral(" (warehouse_code, code, weight, volume) "
                                 "VALUES (:WarehouseCode, :Code, :Weight, :Volume)"));
    query.bindValue(QStringLiteral(":WarehouseCode"), warehouseCode);
    query.bindValue(QStringLiteral(":Code"), QString::number(code));
    query.bindValue(QStringLiteral(":Weight"), weight);
    query.bindValue(QStringLiteral(":Volume"), volume);

    if(!query.exec()) {
        setLastError(query.lastError().text());
    }
}

void OrderDB::deleteWarehouse(const uint64_t code)
{
    QSqlQuery query(db);
    query.prepare(QStringLiteral("DELETE FROM ") + WAREHOUSE_TABLE + QStringLiteral(" WHERE code = :Code"));
    query.bindValue(QStringLiteral(":Code"), QString::number(code));

    if(!query.exec()) {
        setLastError(query.lastError().text());
    }
}

void OrderDB::deleteDestination(const uint64_t code)
{
    QSqlQuery query(db);
    query.prepare(QStringLiteral("DELETE FROM ") + DESTINATION_TABLE + QStringLiteral(" WHERE code = :Code"));
    query.bindValue(QStringLiteral(":Code"), QString::number(code));

    if(!query.exec()) {
        setLastError(query.lastError().text());
    }
}

bool OrderDB::initializeSchema()
{
    if(!tableExists(QStringLiteral(MAIN_TABLE))) {
        return createTables();
    }

    const int version = schemaVersion();
    if(version == 1) {
        return true;
    }

    if(version == 0) {
        return migrateLegacySchema();
    }

    setLastError(QStringLiteral("Unsupported database schema version: %1").arg(version));
    return false;
}

bool OrderDB::createTables()
{
    QSqlQuery query(db);

    if(!exec(query, QStringLiteral("CREATE TABLE IF NOT EXISTS ") + MAIN_TABLE + QStringLiteral(" ("
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                   "color TEXT, "
                                   "start_p TEXT, "
                                   "end_p TEXT, "
                                   "start_time TEXT, "
                                   "distance REAL DEFAULT 0, "
                                   "end_time TEXT, "
                                   "code TEXT UNIQUE NOT NULL)"))) {
        return false;
    }

    if(!exec(query, QStringLiteral("CREATE TABLE IF NOT EXISTS ") + PATH_TABLE + QStringLiteral(" ("
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                   "main_code TEXT NOT NULL UNIQUE, "
                                   "path TEXT, "
                                   "FOREIGN KEY (main_code) REFERENCES ") + MAIN_TABLE +
                                       QStringLiteral("(code) ON DELETE CASCADE)"))) {
        return false;
    }

    if(!exec(query, QStringLiteral("CREATE TABLE IF NOT EXISTS ") + WAREHOUSE_TABLE + QStringLiteral(" ("
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                   "code TEXT UNIQUE NOT NULL, "
                                   "position TEXT NOT NULL)"))) {
        return false;
    }

    if(!exec(query, QStringLiteral("CREATE TABLE IF NOT EXISTS ") + DESTINATION_TABLE + QStringLiteral(" ("
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                   "code TEXT UNIQUE NOT NULL, "
                                   "position TEXT NOT NULL)"))) {
        return false;
    }

    if(!exec(query, QStringLiteral("CREATE TABLE IF NOT EXISTS ") + CARGO_TABLE + QStringLiteral(" ("
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                   "warehouse_code TEXT NOT NULL, "
                                   "code TEXT NOT NULL, "
                                   "weight REAL NOT NULL, "
                                   "volume INTEGER NOT NULL, "
                                   "FOREIGN KEY (warehouse_code) REFERENCES ") + WAREHOUSE_TABLE +
                                       QStringLiteral("(code) ON DELETE CASCADE)"))) {
        return false;
    }

    return setSchemaVersion(1);
}

bool OrderDB::migrateLegacySchema()
{
    db.close();

    if(!backupDatabase(dbPath_)) {
        return false;
    }

    if(!db.open()) {
        setLastError(db.lastError().text());
        return false;
    }

    QSqlQuery pragma(db);
    exec(pragma, QStringLiteral("PRAGMA foreign_keys = ON"));
    exec(pragma, QStringLiteral("PRAGMA busy_timeout = 5000"));

    if(!beginTransaction()) {
        return false;
    }

    const QString ordersLegacy = QStringLiteral(MAIN_TABLE) + QStringLiteral("_legacy");
    const QString pathLegacy = QStringLiteral(PATH_TABLE) + QStringLiteral("_legacy");
    const QString warehouseLegacy = QStringLiteral(WAREHOUSE_TABLE) + QStringLiteral("_legacy");
    const QString destinationLegacy = QStringLiteral(DESTINATION_TABLE) + QStringLiteral("_legacy");

    QSqlQuery query(db);
    if(tableExists(QStringLiteral(MAIN_TABLE))) {
        if(!exec(query, QStringLiteral("ALTER TABLE ") + MAIN_TABLE + QStringLiteral(" RENAME TO ") + ordersLegacy)) {
            rollbackTransaction();
            return false;
        }
    }
    if(tableExists(QStringLiteral(PATH_TABLE))) {
        if(!exec(query, QStringLiteral("ALTER TABLE ") + PATH_TABLE + QStringLiteral(" RENAME TO ") + pathLegacy)) {
            rollbackTransaction();
            return false;
        }
    }
    if(tableExists(QStringLiteral(WAREHOUSE_TABLE))) {
        if(!exec(query, QStringLiteral("ALTER TABLE ") + WAREHOUSE_TABLE + QStringLiteral(" RENAME TO ") + warehouseLegacy)) {
            rollbackTransaction();
            return false;
        }
    }
    if(tableExists(QStringLiteral(DESTINATION_TABLE))) {
        if(!exec(query, QStringLiteral("ALTER TABLE ") + DESTINATION_TABLE + QStringLiteral(" RENAME TO ") + destinationLegacy)) {
            rollbackTransaction();
            return false;
        }
    }

    if(!createTables()) {
        rollbackTransaction();
        return false;
    }

    if(tableExists(ordersLegacy)) {
        if(!exec(query, QStringLiteral("INSERT OR REPLACE INTO ") + MAIN_TABLE +
                          QStringLiteral(" (id, color, start_p, end_p, start_time, distance, end_time, code) "
                                         "SELECT id, color, start_p, end_p, start_time, distance, end_time, code "
                                         "FROM ") + ordersLegacy +
                          QStringLiteral(" WHERE code IS NOT NULL AND code <> ''"))) {
            rollbackTransaction();
            return false;
        }
    }

    if(tableExists(pathLegacy)) {
        QSqlQuery legacyPaths(db);
        if(!legacyPaths.exec(QStringLiteral("SELECT main_code, path FROM ") + pathLegacy)) {
            setLastError(legacyPaths.lastError().text());
            rollbackTransaction();
            return false;
        }

        while(legacyPaths.next()) {
            const QString mainCode = legacyPaths.value(0).toString();
            const QString legacyPath = legacyPaths.value(1).toString();

            QSqlQuery orderExists(db);
            orderExists.prepare(QStringLiteral("SELECT 1 FROM ") + MAIN_TABLE +
                                QStringLiteral(" WHERE code = :Code"));
            orderExists.bindValue(QStringLiteral(":Code"), mainCode);
            if(!orderExists.exec() || !orderExists.next()) {
                continue;
            }

            const QVector<QGeoCoordinate> points = deserializePath(legacyPath);
            QSqlQuery insertPath(db);
            insertPath.prepare(QStringLiteral("INSERT OR REPLACE INTO ") + PATH_TABLE +
                               QStringLiteral(" (main_code, path) VALUES (:Code, :Path)"));
            insertPath.bindValue(QStringLiteral(":Code"), mainCode);
            insertPath.bindValue(QStringLiteral(":Path"), serializePath(points));
            if(!insertPath.exec()) {
                setLastError(insertPath.lastError().text());
                rollbackTransaction();
                return false;
            }
        }
    }

    if(tableExists(warehouseLegacy)) {
        if(!exec(query, QStringLiteral("INSERT OR REPLACE INTO ") + WAREHOUSE_TABLE +
                          QStringLiteral(" (id, code, position) SELECT id, code, position FROM ") +
                          warehouseLegacy)) {
            rollbackTransaction();
            return false;
        }
    }

    if(tableExists(destinationLegacy)) {
        if(!exec(query, QStringLiteral("INSERT OR REPLACE INTO ") + DESTINATION_TABLE +
                          QStringLiteral(" (id, code, position) SELECT id, code, position FROM ") +
                          destinationLegacy)) {
            rollbackTransaction();
            return false;
        }
    }

    if(tableExists(ordersLegacy)) {
        exec(query, QStringLiteral("DROP TABLE ") + ordersLegacy);
    }
    if(tableExists(pathLegacy)) {
        exec(query, QStringLiteral("DROP TABLE ") + pathLegacy);
    }
    if(tableExists(warehouseLegacy)) {
        exec(query, QStringLiteral("DROP TABLE ") + warehouseLegacy);
    }
    if(tableExists(destinationLegacy)) {
        exec(query, QStringLiteral("DROP TABLE ") + destinationLegacy);
    }

    return commitTransaction();
}

bool OrderDB::tableExists(const QString& table) const
{
    QSqlQuery query(db);
    query.prepare(QStringLiteral("SELECT name FROM sqlite_master WHERE type = 'table' AND name = :Name"));
    query.bindValue(QStringLiteral(":Name"), table);

    return query.exec() && query.next();
}

int OrderDB::schemaVersion() const
{
    QSqlQuery query(db);
    if(!query.exec(QStringLiteral("PRAGMA user_version")) || !query.next()) {
        return -1;
    }

    return query.value(0).toInt();
}

bool OrderDB::setSchemaVersion(const int version)
{
    QSqlQuery query(db);
    return exec(query, QStringLiteral("PRAGMA user_version = %1").arg(version));
}

bool OrderDB::exec(QSqlQuery& query, const QString& sql)
{
    if(!query.exec(sql)) {
        setLastError(query.lastError().text());
        return false;
    }

    return true;
}

bool OrderDB::beginTransaction()
{
    if(!db.transaction()) {
        setLastError(db.lastError().text());
        return false;
    }

    return true;
}

bool OrderDB::commitTransaction()
{
    if(!db.commit()) {
        setLastError(db.lastError().text());
        return false;
    }

    return true;
}

bool OrderDB::rollbackTransaction()
{
    return db.rollback();
}

QString OrderDB::serializePath(const QVector<QGeoCoordinate>& path) const
{
    QJsonArray array;
    for(const auto& coordinate : path) {
        QJsonArray point;
        point.append(coordinate.latitude());
        point.append(coordinate.longitude());
        array.append(point);
    }

    return QString::fromUtf8(QJsonDocument(array).toJson(QJsonDocument::Compact));
}

QVector<QGeoCoordinate> OrderDB::deserializePath(const QString& text) const
{
    QVector<QGeoCoordinate> path;

    QJsonParseError error;
    const QJsonDocument document = QJsonDocument::fromJson(text.toUtf8(), &error);
    if(error.error == QJsonParseError::NoError && document.isArray()) {
        for(const auto& value : document.array()) {
            const QJsonArray point = value.toArray();
            if(point.size() == 2) {
                path.push_back(QGeoCoordinate(point.at(0).toDouble(), point.at(1).toDouble()));
            }
        }
        return path;
    }

    const QStringList parts = text.split(' ', Qt::SkipEmptyParts);
    for(int index = 0; index + 1 < parts.size(); index += 2) {
        bool latitudeOk = false;
        bool longitudeOk = false;
        const double latitude = parts[index].toDouble(&latitudeOk);
        const double longitude = parts[index + 1].toDouble(&longitudeOk);
        if(latitudeOk && longitudeOk) {
            path.push_back(QGeoCoordinate(latitude, longitude));
        }
    }

    return path;
}

bool OrderDB::backupDatabase(const QString& path)
{
    if(!QFileInfo::exists(path)) {
        return true;
    }

    const QString backupPath = path + QStringLiteral(".bak-") +
                               QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd-HHmmss-zzz"));
    if(!QFile::copy(path, backupPath)) {
        setLastError(QStringLiteral("Cannot create database backup: ") + backupPath);
        return false;
    }

    return true;
}

void OrderDB::setLastError(const QString& error)
{
    lastError_ = error;
}

bool OrderDB::closeDB()
{
    if(!db.isOpen()) {
        return false;
    }

    db.close();
    emit DbIsClose();
    return true;
}

bool OrderDB::importDB()
{
    const QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Open Database"),
                                                          QDir::currentPath(), tr("SQLite databases (*.db)"));
    if(fileName.isEmpty()) {
        return false;
    }

    db.close();
    db.setDatabaseName(fileName);
    if(!db.open()) {
        setLastError(db.lastError().text());
        return false;
    }

    dbPath_ = fileName;
    if(!initializeSchema()) {
        db.close();
        return false;
    }

    emit DbIsOpen();
    return true;
}
