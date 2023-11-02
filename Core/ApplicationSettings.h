#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QString>

#define APPLICATION_NAME   "OctopusLogistics"

class Company;

class ApplicationSettings
{
public:
    ApplicationSettings() = delete;

    static void     setCompanyName(QString name);
    static QString  companyName();
    static bool     companyIsValid();

    static void     setDbName(QString name);
    static QString  dbName();

private:
    static QString makeIniPath(const char* section, const char* key);

private:
    //Sections
    static constexpr const char* sectionCompany_ = "Company";
    static constexpr const char* sectionDb_      = "DB";

    //keys
    static constexpr const char* keyCompanyName_ = "company_name";
    static constexpr const char* keyDbName_      = "db_name";
};

#endif // APPLICATIONSETTINGS_H
