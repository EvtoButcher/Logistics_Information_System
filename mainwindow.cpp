#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QQmlContext>

#include "RouteModel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    route_table_ = new RouteTable(this);
    ui->dockWidget->setWidget(route_table_);

    route_dialog_ = new RouteDialog(this);

    connect(route_table_, &RouteTable::openRouteDialog, this, &MainWindow::openRouteDialog);
    connect(route_dialog_, &RouteDialog::addRouteToTable, route_table_, &RouteTable::onAddRoute);

    ui->quickWidget->rootContext()->setContextProperty("app", &route_table_->getRouteModel());
    ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/map.qml")));
    ui->quickWidget->show();

    route_table_->restoreRoutOnMap(); //TODO: fix incorrect display of routes during
                                      //      deserialization from the database.
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openRouteDialog()
{
    route_dialog_->exec();
}
