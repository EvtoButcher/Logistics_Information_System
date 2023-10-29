#ifndef WAREHOUSECONTROLWIDGET_H
#define WAREHOUSECONTROLWIDGET_H

#include <QWidget>
#include <QDialog>

#include "Headers/Company.h"
#include "Headers/CreateWarehouseDialog.h"

class QTableWidget;
class QPushButton;
//class QQuickWidget;

class WarehouseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WarehouseWidget(/*WarehouseModel& warehouse_model,*/ QWidget *parent = nullptr);

    //QWidget* getSettingsMap();

signals:
    void addWarehouseToCompany(Warehouse*);

public slots:
    void addWarehouse(Warehouse* warehouse);

private slots:
    void createNewWarehouseDilogOpen();
    void warehouseSelectedOnTable();
    void deleteWarehouseButtotClicked();

private:
    QTableWidget*           warehouses_table_;
    QTableWidget*           curgo_from_warehouses_table_;
    QTableWidget*           curgo_table_;
    QPushButton*            add_warehouse_button_;
    QPushButton*            delete_warehouse_button_;
    CreateWarehouseDialog*  create_warehouse_dialog;
    //QQuickWidget*           settings_map_;

    std::unordered_map<QString, const Warehouse*> warehouses_table_cahe_;
};

#endif // WAREHOUSECONTROLWIDGET_H
