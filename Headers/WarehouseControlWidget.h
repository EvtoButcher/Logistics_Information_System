#ifndef WAREHOUSECONTROLWIDGET_H
#define WAREHOUSECONTROLWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QString>
//#include <QQuickWidget>
#include <Headers/Company.h>

class QTableWidget;
class QPushButton;
class QLineEdit;
class QLabel;

class CreateWarehouseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CreateWarehouseDialog(QWidget* parent = nullptr);

signals:
    void CreateWarehouse(Warehouse*);

private slots:
    void warehouseLatAdding();
    void warehouseLngAdding();
    void tryCreateWarehouse();

    void addCurgo();

    void createWarehouseButtonClicked();

private:
    Warehouse* warehouse_;

    QLineEdit* warehouse_code_;
    QLineEdit* warehouse_lat_;
    QLineEdit* warehouse_lng_;
    QLineEdit* curgo_code_line_edit_;
    QLineEdit* curgo_weight_line_edit_;
    QLineEdit* curgo_volume_line_edit_;
    QLabel* messege_label;
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
    void addNewWarehouse();

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
