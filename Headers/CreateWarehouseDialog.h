#ifndef CREATEWAREHOUSEDIALOG_H
#define CREATEWAREHOUSEDIALOG_H

#include <QDialog>

class Warehouse;
class QLabel;
class QLineEdit;
class QTableWidget;
class QPushButton;
class QQuickWidget;

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

    //QQuickWidget*   settings_map_;
};

#endif // CREATEWAREHOUSEDIALOG_H
