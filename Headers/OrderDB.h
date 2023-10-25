#ifndef ORDERDB_H
#define ORDERDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QColor>
#include <optional>

#include "RouteModel.h"
#include "ApplicationSettings.h"

class QSqlQuery;

#define DATABASE_NAME       "orderDB.db"//
#define MAIN_TABLE          "Orders"    //TODO: replace with file.conf
#define PATH_TABLE          "PathTable" //

enum class ConnectionStatus{
    Offline,
    Online
};

class OrderDB : public QObject
{
    Q_OBJECT

public:
    OrderDB(const ApplicationSettings& setting, QObject *parent = nullptr);
    ~OrderDB() = default;

    const QSqlDatabase& DB() const;

    void inserIntoOrderTable(const RouteInfo& info);
    void updateDistanceFromOrderTable(const QString& code, const int distance);
    void deleteFromOrderTable(const int index);

    void updateColor(const int index, const QString& color);
    QColor selectColor(const int index);

    bool closeDB();
    bool importDB();

    void insrtrIntoPathTable(const QString& main_code, const QVector<QGeoCoordinate>& position_cahe);
    const QString selectPath(const QString code);

signals:
    void DbIsOpen();
    void DbIsClose();

private:
    std::optional<QSqlError> createTables();
    bool openDB(QString db_name);
    void createDB();

    void deleteFromPathTable(const int index);

private:
    QSqlDatabase db;
    QSqlQuery*   query;
};

#endif // ORDERDB_H
