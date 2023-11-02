#include <QQmlContext>
#include <QMessageBox>
#include <QDesktopServices>
#include <QFuture>
#include <QtConcurrent>

#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "RouteModel.h"
#include "TextMessage.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , map_engine_(new MapItemEngine(parent))
    , order_add_(new OrderAddWidget(parent))
    , order_dialog_(new OrderAddDialog(parent))
    , company_(new Company(this))
{
    ui->setupUi(this);
    setWindowTitle(APPLICATION_NAME);

    ui->quickWidget->rootContext()->setContextProperty("route_engine", &map_engine_->getRouteModel());
    ui->quickWidget->rootContext()->setContextProperty("warehouse_engine", &map_engine_->getWarehouseModel());
    ui->quickWidget->rootContext()->setContextProperty("destination_engine", &map_engine_->getDestinationModel());
    ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/OrderMap.qml")));

    create_company_dialog_ = new CreateCompanyDialog(map_engine_->getDestinationModel(), map_engine_->getWarehouseModel(), this);
    connect(create_company_dialog_, &CreateCompanyDialog::addWarehouseOnMap, map_engine_, &MapItemEngine::addWarehouse);
    connect(create_company_dialog_, &CreateCompanyDialog::addDestinationOnMap, map_engine_, &MapItemEngine::addDestination);
    connect(company_, &Company::addedNewWarehouse, order_add_, &OrderAddWidget::addNewWarehouseVariant);
    connect(company_, &Company::addedNewDestination, order_add_, &OrderAddWidget::addNewDestinationVariant);
    connect(company_, &Company::addedNewWarehouse, order_dialog_->getOrderAddWidget(), &OrderAddWidget::addNewWarehouseVariant);
    connect(company_, &Company::addedNewDestination, order_dialog_->getOrderAddWidget(), &OrderAddWidget::addNewDestinationVariant);

    map_engine_->OpenDb(ApplicationSettings::dbName());

    if(!ApplicationSettings::companyIsValid()){
         create_company_dialog_->exec();
         company_ = create_company_dialog_->getCompany();
         ApplicationSettings::setCompanyName(company_->getName());
         //ApplicationSettings::setDbName(...);     //TODO: add ability to write db name in ui
    }
    else {
        //map_engine_->OpenDb(ApplicationSettings::dbName());
        company_->restorCompany(map_engine_->getDB());
    }

    order_table_ = new OrderTable(map_engine_->getDB(), this);
    ui->dockWidget->setWidget(order_table_);

    ui->dockWidget_2->setWidget(order_add_);
    ui->dockWidget_2->hide();

    connect(&map_engine_->getRouteModel(), &RouteModel::selectRouteOnMap, order_table_, &OrderTable::routeOnMapSelected);
    connect(&map_engine_->getRouteModel(), &RouteModel::unselectRouteOnMap, order_table_, &OrderTable::routeOnMapUnselected);
    connect(ui->dockWidget_2, &QDockWidget::visibilityChanged,order_table_, &OrderTable::orderAddWidgetIsVisible);
    connect(order_table_, &OrderTable::openOrderDialog, [=](){order_dialog_->exec();});
    connect(order_table_, &OrderTable::removeRoute, map_engine_, &MapItemEngine::removeRoute);
    connect(order_table_, &OrderTable::onSelectRouteOnTable,
            [=](const int index){emit map_engine_->getRouteModel().selectRouteOnTable(index);});
    connect(order_table_, &OrderTable::onUnselectRouteOnTable,
            [=](){emit map_engine_->getRouteModel().unSelectRouteOnTable();});
    connect(order_table_, &OrderTable::updateColor, map_engine_, &MapItemEngine::routeColorUpdate);
    connect(order_add_, &OrderAddWidget::addRoute, map_engine_, &MapItemEngine::addRoute);
    connect(order_add_, &OrderAddWidget::addRoute, order_table_, &OrderTable::updateTable);
    connect(order_dialog_, &OrderAddDialog::addRouteToTableFromDialog, map_engine_, &MapItemEngine::addRoute);
    //connect(order_dialog_, &OrderAddDialog::addRouteToTableFromDialog, order_table_, &OrderTable::updateTable);
    connect(map_engine_, &MapItemEngine::distanceUpdated, order_table_, &OrderTable::updateTable);

    QFuture<void> future = QtConcurrent::run(map_engine_, &MapItemEngine::restoreMap);
}

MainWindow::~MainWindow()
{
    delete ui;
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
    ui->menuVievShowRouteTable->isChecked() ? ui->dockWidget->show() : ui->dockWidget->hide();
}


void MainWindow::on_menuFileCloseDB_triggered()
{
    //order_table_->closeDbConnection();
}


void MainWindow::on_menuFileOpenDB_triggered()
{
    //order_table_->importFromDB();
}


void MainWindow::on_menuViewShowRouteAdd_triggered()
{
    ui->menuViewShowRouteAdd->isChecked() ? ui->dockWidget_2->show() : ui->dockWidget_2->hide();
}

void MainWindow::on_menuCreateCompany_triggered()
{
    create_company_dialog_->exec();
}


