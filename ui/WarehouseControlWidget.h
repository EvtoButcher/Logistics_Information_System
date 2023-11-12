#ifndef WAREHOUSECONTROLWIDGET_H
#define WAREHOUSECONTROLWIDGET_H

#include <QWidget>
#include <QDialog>

#include "Company.h"
#include "CreateWarehouseDialog.h"

class QTableWidget;
class QPushButton;

class WarehouseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WarehouseWidget(QWidget* parent = nullptr);

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

    std::unordered_map<QString, const Warehouse*> warehouses_table_cahe_;
};

#endif // WAREHOUSECONTROLWIDGET_H
