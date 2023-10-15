#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QQmlContext>
#include <QMessageBox>
#include <QDesktopServices>
#include <QFuture>
#include <QtConcurrent>

#include "RouteModel.h"
#include "TextMessage.h"

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

    //QFuture<void> future = QtConcurrent::run(restoreRoutOnMap, route_table_);

    QFuture<void> future = QtConcurrent::run(route_table_, &RouteTable::restoreRoutOnMap);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openRouteDialog()
{
    route_dialog_->exec();
}

void MainWindow::on_menuFileExit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_menuHelpAboutProgram_triggered()
{
    QMessageBox::information(this,
                             "About Program",
                             message_text::about_program);
}


void MainWindow::on_menuHelpGoSourse_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/EvtoButcher/Logistics_Information_System/",QUrl::TolerantMode));
}


void MainWindow::on_menuVievShowRouteTable_changed()
{
    qDebug() << ui->menuVievShowRouteTable->isChecked();
    ui->menuVievShowRouteTable->isChecked() ? ui->dockWidget->show() : ui->dockWidget->hide();
}


void MainWindow::on_menuFileCloseDB_triggered()
{
    route_table_->closeDbConnection();
}


void MainWindow::on_menuFileOpenDB_triggered()
{
    route_table_->importFromDB();
}

