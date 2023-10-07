#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QQmlContext>

#include "RouteModel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->verticalLayout->setAlignment(Qt::AlignTop);

    ui->quickWidget->rootContext()->setContextProperty("app", &ui->widget->GetRouteModel());
    ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/map.qml")));
    ui->quickWidget->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
