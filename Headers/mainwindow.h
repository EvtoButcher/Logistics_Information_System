#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "OrderAddDialog.h"
#include "OrderTableWidget.h"
#include "OrderAddWidget.h"

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
    void openRouteDialog();

    void on_menuFileExit_triggered();
    void on_menuHelpAboutProgram_triggered();
    void on_menuHelpGoSourse_triggered();
    void on_menuVievShowRouteTable_changed();


    void on_menuFileCloseDB_triggered();

    void on_menuFileOpenDB_triggered();

    void on_menuViewShowRouteAdd_triggered();

private:
    Ui::MainWindow *ui;

    OrderTable* order_table_;
    OrderAddDialog* order_dialog_;
    OrderAddWidget* order_add_;
};
#endif // MAINWINDOW_H
