#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QWidget>
#include <QSettings>

#define APPLICATION_NAME   "OctopusLogistics"

class ApplicationSettings
{
public:
    explicit ApplicationSettings(QWidget *parent = nullptr);
    ~ApplicationSettings();

    void loadSettings();
    void saveSettings();

    const QString dbName() const;
    const QString companyName() const;

    bool companyIsValid() const;

signals:

private:
    QSettings settings_;
};

#endif // APPLICATIONSETTINGS_H
