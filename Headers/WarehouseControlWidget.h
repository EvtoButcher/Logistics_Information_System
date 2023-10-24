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
    void createWarehouse(Warehouse*);

private slots:
    void warehouseLatAdding();
    void warehouseLngAdding();
    void tryCreateWarehouse();
    void addCurgo();
    void createWarehouseButtonClicked();

private:
    void enabledAddCurgo(bool status);

private:
    Warehouse* warehouse_;

    QLabel*         curgo_table_label_;
    QLabel*         curgo_weight_label_;
    QLabel*         curgo_volume_label_;
    QLineEdit*      warehouse_code_;
    QLineEdit*      warehouse_lat_;
    QLineEdit*      warehouse_lng_;
    QLineEdit*      curgo_weight_line_edit_;
    QLineEdit*      curgo_volume_line_edit_;
    QLabel*         messege_label_;
    QTableWidget*   curgo_table_;
    QPushButton*    create_warehouse_button_;
    QPushButton*    add_curgo_to_warehouse_button_;
};



class WarehouseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WarehouseWidget(QWidget *parent = nullptr);

signals:
    void addWarehouseToCompany(Warehouse*);

public slots:
    void addWarehouse(Warehouse* warehouse);

private slots:
    void createNewWarehouseDilogOpen();
    void warehouseSelectedOnTable();

private:
    QTableWidget*           warehouses_table_;
    QTableWidget*           curgo_from_warehouses_table_;
    QTableWidget*           curgo_table_;
    QPushButton*            add_warehouse_button_;
    QPushButton*            delete_warehouse_button_;
    CreateWarehouseDialog   create_warehouse_dialog;

    std::unordered_map<QString, const Warehouse*> warehouses_table_cahe_;
    //QQuickWidget* map;
};

#endif // WAREHOUSECONTROLWIDGET_H
