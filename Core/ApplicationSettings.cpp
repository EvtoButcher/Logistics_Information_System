#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QStandardPaths>

#include "ApplicationSettings.h"

void ApplicationSettings::setLocale(Locale locale)
{
    QSettings settings;
    settings.setValue(makeIniPath(sectionUi_, keyUiLocale_), locale == Locale::RU ? "ru" : "eng");
}

QString ApplicationSettings::getLocale()
{
    QSettings settings;
    return settings.value(makeIniPath(sectionUi_, keyUiLocale_), "eng").toString();
}

void ApplicationSettings::setCompanyName(const QString& name)
{
    QSettings settings;
    settings.setValue(makeIniPath(sectionCompany_, keyCompanyName_), name);
}

void ApplicationSettings::setDbName(const QString& name)
{
    QSettings settings;
    settings.setValue(makeIniPath(sectionDb_, keyDbName_), name);
}

QString ApplicationSettings::dbName()
{
    QSettings settings;
    return settings.value(makeIniPath(sectionDb_, keyDbName_), defaultDbName_).toString();
}

QString ApplicationSettings::dbPath()
{
    const QString name = dbName();
    const QFileInfo info(name);
    if(info.isAbsolute()) {
        return name;
    }

    return QDir(dataDirectory()).filePath(name);
}

QString ApplicationSettings::companyName()
{
    QSettings settings;
    return settings.value(makeIniPath(sectionCompany_, keyCompanyName_)).toString();
}

bool ApplicationSettings::companyIsValid()
{
    return !companyName().trimmed().isEmpty();
}

QString ApplicationSettings::dataDirectory()
{
    const QString directory = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(directory);
    return directory;
}

QString ApplicationSettings::makeIniPath(const char* section, const char* key)
{
    return QString(section) + "/" + QString(key);
}
