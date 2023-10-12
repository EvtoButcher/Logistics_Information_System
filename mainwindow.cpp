#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QQmlContext>

#include "RouteModel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    connect(ui->widget, &RouteWidget::addRouteToTable, ui->widget_2, &RouteTable::onAddRoute);
    //ui->verticalLayout->setAlignment(Qt::AlignTop);

    ui->quickWidget->rootContext()->setContextProperty("app", &ui->widget_2->getRouteModel());
    ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/map.qml")));
    ui->quickWidget->show();

    ui->widget_2->restoreRoutOnMap(); //TODO: fix incorrect display of routes during
                                      //      deserialization from the database.
}

MainWindow::~MainWindow()
{
    delete ui;
}
