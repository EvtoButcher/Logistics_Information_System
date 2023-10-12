#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "RouteAddDialog.h"
#include "RouteTableWidget.h"

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

private:
    Ui::MainWindow *ui;

    RouteTable* route_table_;
    RouteDialog* route_dialog_;
};
#endif // MAINWINDOW_H
