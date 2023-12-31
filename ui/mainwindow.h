#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "OrderAddDialog.h"
#include "OrderTableWidget.h"
#include "OrderAddWidget.h"
#include "ApplicationSettings.h"
#include "CreateCompanyDialog.h"
#include "Company.h"
#include "MapItemEngine.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_menuFileExit_triggered();
    void on_menuHelpAboutProgram_triggered();
    void on_menuHelpGoSourse_triggered();
    void on_menuVievShowRouteTable_changed();
    void on_menuFileCloseDB_triggered();
    void on_menuFileOpenDB_triggered();
    void on_menuViewShowRouteAdd_triggered();
    void on_menuCreateCompany_triggered();

private:
    Ui::MainWindow *ui;

    MapItemEngine*       map_engine_;
    CreateCompanyDialog* create_company_dialog_;
    OrderTable*          order_table_;
    OrderAddWidget*      order_add_;
    OrderAddDialog*      order_dialog_;
    Company*             company_;
};
#endif // MAINWINDOW_H
