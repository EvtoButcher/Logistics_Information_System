#include <QtTest>
#include <QDir>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSignalSpy>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTemporaryDir>

#include "CarSimulatorModel.h"
#include "OrderDB.h"
#include "RouteModel.h"
#include "common.h"

namespace {

bool createLegacyDatabase(const QString& path)
{
    const QString connectionName = QStringLiteral("legacy_setup");

    bool ok = false;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), connectionName);
        db.setDatabaseName(path);
        if(!db.open()) {
            qWarning() << db.lastError().text();
            return false;
        }

        QSqlQuery query(db);
        ok = query.exec(QStringLiteral(
                 "CREATE TABLE Orders (id INTEGER PRIMARY KEY, color CHAR, start_p CHAR, end_p CHAR, "
                 "start_time TEXT, distance INTEGER, end_time TEXT, code CHAR)")) &&
             query.exec(QStringLiteral(
                 "CREATE TABLE PathTable (id INTEGER PRIMARY KEY, main_code CHAR, path TEXT)")) &&
             query.exec(QStringLiteral(
                 "CREATE TABLE Warehouse_table (id INTEGER PRIMARY KEY, code CHAR, position CHAR)")) &&
             query.exec(QStringLiteral(
                 "CREATE TABLE Destination_table (id INTEGER PRIMARY KEY, code CHAR, position CHAR)"));

        ok = ok && query.exec(QStringLiteral(
                       "INSERT INTO Orders (id, color, start_p, end_p, start_time, distance, end_time, code) "
                       "VALUES (1, '#00ff00', '55.0 37.0', '55.5 37.5', '2024-01-01', 10, '2024-01-01', 'legacy-1')")) &&
             query.exec(QStringLiteral(
                 "INSERT INTO PathTable (main_code, path) VALUES ('legacy-1', '55.0 37.0 55.25 37.25 55.5 37.5 ')"));
        ok = ok && query.exec(QStringLiteral(
                       "INSERT INTO Warehouse_table (code, position) VALUES ('123', '55.0 37.0')")) &&
             query.exec(QStringLiteral(
                 "INSERT INTO Destination_table (code, position) VALUES ('456', '55.5 37.5')"));

        db.close();
    }

    QSqlDatabase::removeDatabase(connectionName);
    return ok;
}

} // namespace

class CoreTest : public QObject
{
    Q_OBJECT

private slots:
    void splitCoordinates();
    void createCode();
    void routeInfoPathFormats();
    void databaseCrud();
    void legacyMigration();
    void simulation();
};

void CoreTest::splitCoordinates()
{
    const QGeoCoordinate valid = common::splitCoordinates(QStringLiteral("55.100000 37.200000"));
    QVERIFY(valid.isValid());
    QCOMPARE(valid.latitude(), 55.1);
    QCOMPARE(valid.longitude(), 37.2);

    QVERIFY(!common::splitCoordinates(QString()).isValid());
    QVERIFY(!common::splitCoordinates(QStringLiteral("not coordinates")).isValid());
}

void CoreTest::createCode()
{
    const QGeoCoordinate first(55.0, 37.0);
    const QGeoCoordinate second(55.0, 37.0);

    const uint64_t firstCode = common::createCode(first);
    QVERIFY(firstCode != 0);
    QCOMPARE(firstCode, common::createCode(second));
    QVERIFY(firstCode != common::createCode(QGeoCoordinate(55.1, 37.1)));

    QVERIFY(common::createCode(10.0, 2) != common::createCode(11.0, 2));
    QVERIFY(common::createCode(10.0, 2) != common::createCode(10.0, 3));
}

void CoreTest::routeInfoPathFormats()
{
    const QGeoCoordinate start(55.0, 37.0);
    const QGeoCoordinate end(55.5, 37.5);

    RouteInfo jsonRoute(QStringLiteral("json"),
                        start,
                        end,
                        QStringLiteral("[[55.0,37.0],[55.25,37.25],[55.5,37.5]]"),
                        QStringLiteral("#00ff00"));
    QCOMPARE(jsonRoute.path_cache_.size(), 3);

    RouteInfo legacyRoute(QStringLiteral("legacy"),
                          start,
                          end,
                          QStringLiteral("55.0 37.0 55.25 37.25 55.5 37.5 "),
                          QStringLiteral("#00ff00"));
    QCOMPARE(legacyRoute.path_cache_.size(), 3);

    RouteInfo brokenRoute(QStringLiteral("broken"),
                          start,
                          end,
                          QStringLiteral("55.0"),
                          QStringLiteral("#00ff00"));
    QVERIFY(brokenRoute.path_cache_.isEmpty());
}

void CoreTest::databaseCrud()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    const QString path = directory.filePath(QStringLiteral("crud.db"));
    OrderDB db(path);
    QVERIFY(db.isOpen());

    QSqlQuery version(db.DB());
    QVERIFY(version.exec(QStringLiteral("PRAGMA user_version")));
    QVERIFY(version.next());
    QCOMPARE(version.value(0).toInt(), 1);

    const RouteInfo info(QStringLiteral("route-1"),
                         QGeoCoordinate(55.0, 37.0),
                         QGeoCoordinate(55.5, 37.5),
                         QString(),
                         QStringLiteral("#00ff00"));
    db.insertIntoOrderTable(info);

    QVector<QGeoCoordinate> points;
    points.push_back(QGeoCoordinate(55.0, 37.0));
    points.push_back(QGeoCoordinate(55.5, 37.5));
    db.insertIntoPathTable(QStringLiteral("route-1"), points);

    const QJsonDocument pathDocument = QJsonDocument::fromJson(db.selectPath(QStringLiteral("route-1")).toUtf8());
    QVERIFY(pathDocument.isArray());
    QCOMPARE(pathDocument.array().size(), 2);

    db.insertIntoWarehouseTable(123, QGeoCoordinate(55.0, 37.0));
    db.insertCargo(QStringLiteral("123"), 555, 12.5, 3);

    const QVector<CargoRecord> cargo = db.selectCargo(QStringLiteral("123"));
    QCOMPARE(cargo.size(), 1);
    QCOMPARE(cargo.at(0).code_, uint64_t(555));
    QCOMPARE(cargo.at(0).weight_, 12.5);
    QCOMPARE(cargo.at(0).volume_, 3);

    db.deleteWarehouse(123);
    QVERIFY(db.selectCargo(QStringLiteral("123")).isEmpty());

    db.deleteFromOrderTable(1);
    QVERIFY(db.selectPath(QStringLiteral("route-1")).isEmpty());
}

void CoreTest::legacyMigration()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    const QString path = directory.filePath(QStringLiteral("legacy.db"));
    QVERIFY(createLegacyDatabase(path));

    OrderDB db(path);
    QVERIFY(db.isOpen());

    QSqlQuery version(db.DB());
    QVERIFY(version.exec(QStringLiteral("PRAGMA user_version")));
    QVERIFY(version.next());
    QCOMPARE(version.value(0).toInt(), 1);

    QSqlQuery tables(db.DB());
    QVERIFY(tables.exec(QStringLiteral("SELECT name FROM sqlite_master WHERE type = 'table'")));
    bool hasCargoTable = false;
    while(tables.next()) {
        hasCargoTable = hasCargoTable || tables.value(0).toString() == QStringLiteral("Cargo_table");
    }
    QVERIFY(hasCargoTable);

    const QJsonDocument pathDocument = QJsonDocument::fromJson(db.selectPath(QStringLiteral("legacy-1")).toUtf8());
    QVERIFY(pathDocument.isArray());
    QCOMPARE(pathDocument.array().size(), 3);

    const QStringList backups = QDir(directory.path()).entryList(QStringList() << QStringLiteral("*.bak-*"));
    QVERIFY(!backups.isEmpty());
}

void CoreTest::simulation()
{
    CarSimulatorModel model;

    QVector<QGeoCoordinate> path;
    path.push_back(QGeoCoordinate(55.00, 37.00));
    path.push_back(QGeoCoordinate(55.01, 37.01));
    path.push_back(QGeoCoordinate(55.02, 37.02));
    path.push_back(QGeoCoordinate(55.03, 37.03));
    model.addTraffic(TrafficInfo(path, QStringLiteral("#ffffff")));

    QSignalSpy nextPointSpy(&model, &AbstractTrifficModel::nextPoint);
    QSignalSpy finishedSpy(&model, &AbstractTrifficModel::simulationFinished);
    QSignalSpy clearSpy(&model, &AbstractTrifficModel::clearCars);

    model.startSimulation(10);
    QVERIFY(finishedSpy.wait(3000));
    QVERIFY(nextPointSpy.count() >= 2);
    QCOMPARE(clearSpy.count(), 1);

    model.resetSimulation();
    QCOMPARE(model.getTraffic().size(), 1);
}

QTEST_GUILESS_MAIN(CoreTest)

#include "tst_core.moc"
