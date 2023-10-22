#include <QDebug>

#include "Headers/ApplicationSettings.h"

ApplicationSettings::ApplicationSettings(QWidget *parent)
    : settings_(APPLICATION_NAME, QSettings::Format::IniFormat, parent)
{

    //settings_.setValue("DB/ConnectionStatus", 0);
    //settings_.setValue("DB/name", "orderDB.db");

    loadSettings();
}

ApplicationSettings::~ApplicationSettings()
{
    saveSettings();
}

void ApplicationSettings::loadSettings()
{

}

void ApplicationSettings::saveSettings()
{

}

const QString ApplicationSettings::dbName() const
{
    qDebug() << settings_.value("DB/name").toString();
    return settings_.value("DB/name").toString();
}
