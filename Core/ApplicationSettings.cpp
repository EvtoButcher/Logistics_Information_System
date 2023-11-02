#include <QDebug>
#include <QSettings>

#include "ApplicationSettings.h"
#include "Company.h"

void ApplicationSettings::setLocale(Locale locale)
{
    QSettings(APPLICATION_NAME, QSettings::Format::IniFormat).setValue(makeIniPath(sectionUi_, keyUiLocale_),
                                                                       locale == Locale::RU ? "ru" : "eng");
}

QString ApplicationSettings::getLocale()
{
    return QSettings(APPLICATION_NAME, QSettings::Format::IniFormat).value(makeIniPath(sectionUi_, keyUiLocale_)).toString();
}

void ApplicationSettings::setCompanyName(const QString& name)
{
    QSettings(APPLICATION_NAME, QSettings::Format::IniFormat).setValue(makeIniPath(sectionCompany_, keyCompanyName_), name);
}

void ApplicationSettings::setDbName(const QString& name)
{
    QSettings(APPLICATION_NAME, QSettings::Format::IniFormat).setValue(makeIniPath(sectionDb_, keyDbName_), name);
}

QString ApplicationSettings::dbName()
{
    return QSettings(APPLICATION_NAME, QSettings::Format::IniFormat).value(makeIniPath(sectionDb_, keyDbName_)).toString();
}

QString ApplicationSettings::companyName()
{
    return QSettings(APPLICATION_NAME, QSettings::Format::IniFormat).value(makeIniPath(sectionCompany_, keyCompanyName_)).toString();
}

bool ApplicationSettings::companyIsValid()
{
    return companyName() != "";
}

QString ApplicationSettings::makeIniPath(const char* section, const char* key)
{
    return QString(section) + "/" + QString(key);
}
