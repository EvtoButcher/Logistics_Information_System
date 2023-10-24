#include <QQmlContext>
#include <QMessageBox>
#include <QDesktopServices>
#include <QFuture>
#include <QtConcurrent>

#include "ui_mainwindow.h"
#include "Headers/mainwindow.h"
#include "Headers/RouteModel.h"
#include "Headers/TextMessage.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,settings_(parent)
{
    ui->setupUi(this);

    setWindowTitle(APPLICATION_NAME);
    //settings_ = ApplicationSettings(this);

    create_company_dialog_ = new CreateCompanyDialog(this);

    order_table_ = new OrderTable(settings_, this);
    ui->dockWidget->setWidget(order_table_);

    order_add_ = new OrderAddWidget(this);
    ui->dockWidget_2->setWidget(order_add_);
    ui->dockWidget_2->hide();

    order_dialog_ = new OrderAddDialog(this);

    connect(ui->dockWidget_2, &QDockWidget::visibilityChanged,order_table_, &OrderTable::orderAddWidgetIsVisible);
    connect(order_table_, &OrderTable::openOrderDialog, this, &MainWindow::openRouteDialog);
    connect(order_add_, &OrderAddWidget::addRouteToTable, order_table_, &OrderTable::onAddOrder);
    connect(order_dialog_, &OrderAddDialog::addRouteToTableFromDialog, order_table_, &OrderTable::onAddOrder);

    ui->quickWidget->rootContext()->setContextProperty("app", &order_table_->getRouteModel());
    ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/map.qml")));
    ui->quickWidget->show();

    if(!settings_.companyIsValid()){
         create_company_dialog_->exec();
    }
    else {
        QFuture<void> future = QtConcurrent::run(order_table_, &OrderTable::restoreRoutOnMap);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openRouteDialog()
{
    order_dialog_->exec();
}

void MainWindow::on_menuFileExit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_menuHelpAboutProgram_triggered()
{
    QMessageBox::information(this, "About Program", message_text::about_program);
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
    order_table_->closeDbConnection();
}


void MainWindow::on_menuFileOpenDB_triggered()
{
    order_table_->importFromDB();
}


void MainWindow::on_menuViewShowRouteAdd_triggered()
{
    ui->menuViewShowRouteAdd->isChecked() ? ui->dockWidget_2->show() : ui->dockWidget_2->hide();

    //ui->dockWidget_2->visibilityChanged()
}

void MainWindow::on_menuCreateCompany_triggered()
{
    create_company_dialog_->exec();
}

