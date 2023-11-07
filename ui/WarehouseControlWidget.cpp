#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFont>
#include <QTableWidget>
#include <QStringList>
#include <QPushButton>
#include <QQuickWidget>
#include <QQmlContext>

#include <QDebug>

#include "WarehouseControlWidget.h"

WarehouseWidget::WarehouseWidget(/*WarehouseModel& warehouse_model,*/ QWidget *parent)
    : QWidget{parent}
    //, settings_map_(new QQuickWidget(parent))
{
    auto bold_font = QFont("Ubuntu", 11, QFont::Bold);

    auto create_warehouses_label = new QLabel(tr("Create warehouses"), this);
    create_warehouses_label->setFont(bold_font);

    auto warehous_table_label = new QLabel(tr("Warehouses"), this);
    warehouses_table_ = new QTableWidget(0, 2, this);
    QStringList warehous_table_colum;
    warehous_table_colum << tr("code") << tr("position");
    warehouses_table_->setHorizontalHeaderLabels(warehous_table_colum);
    auto warehouses_table_lay = new QVBoxLayout();
    warehouses_table_lay->addWidget(warehous_table_label);
    warehouses_table_lay->setSpacing(5);
    warehouses_table_lay->addWidget(warehouses_table_);

    auto cargo_from_warehouses_table_label = new QLabel(tr("Cargo"), this);
    curgo_from_warehouses_table_ = new QTableWidget(0, 3, this);
    QStringList cargo_from_warehouses_table_colum;
    cargo_from_warehouses_table_colum << tr("code") << tr("Weight") << tr("Volume");
    curgo_from_warehouses_table_->setHorizontalHeaderLabels(cargo_from_warehouses_table_colum);
    auto cargo_from_warehouses_lay = new QVBoxLayout();
    cargo_from_warehouses_lay->addWidget(cargo_from_warehouses_table_label);
    cargo_from_warehouses_lay->setSpacing(5);
    cargo_from_warehouses_lay->addWidget(curgo_from_warehouses_table_);

    auto table_lay = new QHBoxLayout();
    table_lay->addItem(warehouses_table_lay);
    table_lay->addSpacing(20);
    table_lay->addItem(cargo_from_warehouses_lay);

    auto button_lay = new QHBoxLayout();
    add_warehouse_button_ = new QPushButton(tr("add a new warehouse"), this);
    delete_warehouse_button_ = new QPushButton(tr("delete a warehouse"), this);
    delete_warehouse_button_->setStyleSheet("QPushButton {color: rgba(183, 22, 26, 80);}");
    delete_warehouse_button_->setEnabled(false);
    button_lay->addWidget(add_warehouse_button_);
    button_lay->addSpacing(10);
    button_lay->addWidget(delete_warehouse_button_);

    setLayout(new QVBoxLayout(this));

    layout()->addWidget(create_warehouses_label);
    layout()->setSpacing(10);
    layout()->addItem(table_lay);
    layout()->addItem(button_lay);
    layout()->setMargin(0);

    //settings_map_->setMinimumSize(QSize(700, 700));
    //settings_map_->rootContext()->setContextProperty(CONTEXT_WAREHOUSE_NAME, &warehouse_model);
    //settings_map_->setSource(QUrl(QStringLiteral("qrc:/SettingMap.qml")));

    create_warehouse_dialog = new CreateWarehouseDialog(/*settings_map_,*/ this);

    connect(add_warehouse_button_, &QAbstractButton::clicked, this, &WarehouseWidget::createNewWarehouseDilogOpen);
    connect(delete_warehouse_button_, &QAbstractButton::clicked, this, &WarehouseWidget::deleteWarehouseButtotClicked);
    connect(create_warehouse_dialog, &CreateWarehouseDialog::createWarehouse, this, &WarehouseWidget::addWarehouse);
    connect(warehouses_table_->selectionModel(), &QItemSelectionModel::selectionChanged, this, &WarehouseWidget::warehouseSelectedOnTable);
}

//QWidget* WarehouseWidget::getSettingsMap()
//{
//    return settings_map_;
//}

void WarehouseWidget::addWarehouse(Warehouse *warehouse)
{
    emit addWarehouseToCompany(warehouse);

//    warehouse_model_.setWarehouse({warehouse->getCode(), warehouse->getPosition()});
//    emit warehouse_model_.addWarehouse();

    int row_count = warehouses_table_->rowCount();
    warehouses_table_->setRowCount(row_count + 1);

    for(int colum_index = 0; colum_index < warehouses_table_->columnCount(); ++colum_index){
        auto* newItem = new QTableWidgetItem();
        switch (colum_index) {
            case 0:
                newItem->setText("#" + QString::number(warehouse->getCode())); /*Code*/
                break;
            case 1:
                newItem->setText(warehouse->getPosition().toString()); /*Weight*/
                break;
        }
        warehouses_table_->setItem(row_count, colum_index, newItem);
        warehouses_table_->resizeColumnToContents(1);
    }

    if(curgo_from_warehouses_table_->rowCount()){
        curgo_from_warehouses_table_->setRowCount(0);
    }
    for(const auto& [code, curgo] : warehouse->getCurgo()){
        row_count = curgo_from_warehouses_table_->rowCount();
        curgo_from_warehouses_table_->setRowCount(row_count + 1);

        for(int colum_index = 0; colum_index < curgo_from_warehouses_table_->columnCount(); ++colum_index){
            auto* newItem = new QTableWidgetItem();
            switch (colum_index) {
                case 0:
                    newItem->setText("#" + QString::number(curgo->code_)); /*Code*/
                    break;
                case 1:
                    newItem->setText(QString::number(curgo->weight_) + tr(" kg")); /*Weight*/
                    break;
                case 2:
                    newItem->setText(QString::number(curgo->volume_) + tr(" сm³")); /*Volume*/
                    break;
            }
            curgo_from_warehouses_table_->setItem(row_count, colum_index, newItem);
            curgo_from_warehouses_table_->resizeColumnToContents(1);
            curgo_from_warehouses_table_->resizeColumnToContents(2);
        }
    }

    warehouses_table_cahe_[QString::number(warehouse->getCode())] = warehouse;
}

void WarehouseWidget::createNewWarehouseDilogOpen()
{
    create_warehouse_dialog->exec();
}

void WarehouseWidget::warehouseSelectedOnTable()
{
    if(curgo_from_warehouses_table_->rowCount()){
        curgo_from_warehouses_table_->setRowCount(0);
    }

    QItemSelectionModel* selectionModel = warehouses_table_->selectionModel();
    QModelIndexList indexes = selectionModel->selectedRows();

    if(!indexes.empty()) {
         delete_warehouse_button_->setEnabled(true);
         delete_warehouse_button_->setStyleSheet("QPushButton {color: rgb(183, 22, 26);}");
    }
    else {
        delete_warehouse_button_->setEnabled(false);
        delete_warehouse_button_->setStyleSheet("QPushButton {color: rgba(183, 22, 26, 80);}");
        return;
    }

    for(const auto [code, curgo] : warehouses_table_cahe_[indexes[0].data(Qt::DisplayRole).toString().remove(0,1)]->getCurgo()){
        int row_count = curgo_from_warehouses_table_->rowCount();
        curgo_from_warehouses_table_->setRowCount(row_count + 1);

        for(int colum_index = 0; colum_index < curgo_from_warehouses_table_->columnCount(); ++colum_index){
            auto* newItem = new QTableWidgetItem();
            switch (colum_index) {
                case 0:
                    newItem->setText("#" + QString::number(code)); /*Code*/
                    break;
                case 1:
                    newItem->setText(QString::number(curgo->weight_) + tr(" kg")); /*Weight*/
                    break;
                case 2:
                    newItem->setText(QString::number(curgo->volume_) + tr(" сm³")); /*Volume*/
                    break;
            }
            curgo_from_warehouses_table_->setItem(row_count, colum_index, newItem);
            curgo_from_warehouses_table_->resizeColumnsToContents();
        }
    }

}

void WarehouseWidget::deleteWarehouseButtotClicked()
{
    QItemSelectionModel* selectionModel = warehouses_table_->selectionModel();
    QModelIndexList indexes = selectionModel->selectedRows();

    if(indexes.isEmpty()){
        return; //TODO: add error handling
    }

    warehouses_table_->removeRow(indexes.at(0).row());

    warehouses_table_cahe_.erase(indexes[0].data(Qt::DisplayRole).toString().remove(0,1));
}
