#include <QApplication>
#include <QCoreApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(QStringLiteral("TheKramarch"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("github.com/TheKramarch"));
    QCoreApplication::setApplicationName(QStringLiteral("LogisticsInformationSystem"));
    QCoreApplication::setApplicationVersion(QStringLiteral(APP_VERSION));

    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return a.exec();
}
