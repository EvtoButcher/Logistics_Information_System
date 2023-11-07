#include <QApplication>
#include <QTranslator>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QTranslator translater;
//    translater.load(QString(":/OctopusLogistics_%1.qm").arg(ApplicationSettings::getLocale()));
//    a.installTranslator(&translater);

    MainWindow w;
    w.show();
    return a.exec();
}
