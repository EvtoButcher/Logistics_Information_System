#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QString>

#define APPLICATION_NAME   "OctopusLogistics"

class Company;

class ApplicationSettings
{
public:
    ApplicationSettings() = delete;

    enum class Locale{    RU,    ENG    };

    static void     setLocale(Locale locale);
    static QString  getLocale();

    static void     setCompanyName(const QString& name);
    static QString  companyName();
    static bool     companyIsValid();

    static void     setDbName(const QString& name);
    static QString  dbName();

private:
    static QString makeIniPath(const char* section, const char* key);

private:
    //Sections
    static constexpr const char* sectionUi_       = "Ui";
    static constexpr const char* sectionCompany_ = "Company";
    static constexpr const char* sectionDb_      = "DB";

    //keys
    static constexpr const char* keyUiLocale_     = "Locale";
    static constexpr const char* keyCompanyName_ = "company_name";
    static constexpr const char* keyDbName_      = "db_name";
};

#endif // APPLICATIONSETTINGS_H
