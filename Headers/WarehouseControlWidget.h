#ifndef WAREHOUSECONTROLWIDGET_H
#define WAREHOUSECONTROLWIDGET_H

#include <QWidget>
#include <QDialog>
//#include <QQuickWidget>
#include <Headers/Company.h>

class QTableWidget;
class QPushButton;
class QLineEdit;

class CreateWarehouseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CreateWarehouseDialog(QWidget* parent = nullptr);

signals:
    void CreateWarehouse(Warehouse*);

private slots:
    void createWarehouseButtonClicked();

private:
    QLineEdit* warehouse_code_;
    QLineEdit* warehouse_lat_;
    QLineEdit* warehouse_lng_;
    QTableWidget* curgo_table_;
    QPushButton* create_warehouse_button_;
    QPushButton* add_curgo_to_warehouse_button_;
};

class WarehouseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WarehouseWidget(QWidget *parent = nullptr);

signals:

private slots:
    void add_new_warehouse();

private:
    QTableWidget* warehouses_table_;
    QTableWidget* curgo_from_warehouses_table_;
    QTableWidget* curgo_table_;

    QPushButton* add_warehouse_button_;
    QPushButton* delete_warehouse_button_;
    CreateWarehouseDialog create_warehouse_dialog;

    //QQuickWidget* map;

};

#endif // WAREHOUSECONTROLWIDGET_H
