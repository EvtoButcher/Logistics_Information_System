#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFont>
#include <QTableWidget>
#include <QStringList>
#include <QPushButton>

#include "Headers/WarehouseControlWidget.h"

WarehouseWidget::WarehouseWidget(QWidget *parent)
    : QWidget{parent}
    , create_warehouse_dialog(parent)
{
    //create_warehouse_dialog.setModal(true);

    auto bold_font = QFont("Ubuntu", 11, QFont::Bold);

    auto create_warehouses_label = new QLabel("Create warehouses", this);
    create_warehouses_label->setFont(bold_font);

    auto warehous_table_label = new QLabel("Warehouses", this);
    warehouses_table_ = new QTableWidget(0, 2, this);
    QStringList warehous_table_colum;
    warehous_table_colum << "code" << "position";
    warehouses_table_->setHorizontalHeaderLabels(warehous_table_colum);
    auto warehouses_table_lay = new QVBoxLayout();
    warehouses_table_lay->addWidget(warehous_table_label);
    warehouses_table_lay->setSpacing(5);
    warehouses_table_lay->addWidget(warehouses_table_);

    auto cargo_from_warehouses_table_label = new QLabel("Cargo", this);
    curgo_from_warehouses_table_ = new QTableWidget(0, 3, this);
    QStringList cargo_from_warehouses_table_colum;
    cargo_from_warehouses_table_colum << "name" << "Weight" << "Volume";
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
    add_warehouse_button_ = new QPushButton("add a new warehouse", this);
    delete_warehouse_button_ = new QPushButton("delete a warehouse", this);
    delete_warehouse_button_->setStyleSheet("QPushButton {color: #b7161a;}");
    button_lay->addWidget(add_warehouse_button_);
    button_lay->addSpacing(10);
    button_lay->addWidget(delete_warehouse_button_);

//    map = new QQuickWidget(this);
//    map->setSource(QUrl(QStringLiteral("qrc:/map.qml")));

    setLayout(new QVBoxLayout(this));

    layout()->addWidget(create_warehouses_label);
    layout()->setSpacing(10);
    layout()->addItem(table_lay);
    layout()->addItem(button_lay);


    //layout()->addWidget(map);

    layout()->setMargin(0);
    //layout()->setSpacing(0);

    connect(add_warehouse_button_, &QAbstractButton::clicked, this, &WarehouseWidget::add_new_warehouse);
}

void WarehouseWidget::add_new_warehouse()
{
    create_warehouse_dialog.exec();
}

CreateWarehouseDialog::CreateWarehouseDialog(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle("Create Warehouse");

    auto bold_font = QFont("Ubuntu", 11, QFont::Bold);

    auto warehouse_code_label = new QLabel("Warehouse code:", this);
    warehouse_code_label->setFont(bold_font);
    warehouse_code_ = new QLineEdit(this);

    auto warehouse_position_label = new QLabel("Position:", this);
    warehouse_position_label->setFont(bold_font);
    auto warehouse_lat_label = new QLabel("latitude:", this);
    warehouse_lat_ = new QLineEdit(this);
    auto warehouse_lng_label = new QLabel("longitude:", this);
    warehouse_lng_ = new QLineEdit(this);

    auto warehouse_position_lay = new QHBoxLayout();
    warehouse_position_lay->addWidget(warehouse_lat_label);
    warehouse_position_lay->addSpacing(10);
    warehouse_position_lay->addWidget(warehouse_lat_);
    warehouse_position_lay->addSpacing(10);
    warehouse_position_lay->addWidget(warehouse_lng_label);
    warehouse_position_lay->addSpacing(10);
    warehouse_position_lay->addWidget(warehouse_lng_);


    auto curgo_table_label = new QLabel("Added Cargo", this);
    curgo_table_ = new QTableWidget(0, 3, this);
    QStringList curgo_table_colum;
    curgo_table_colum << "name" << "Weight" << "Volume";
    curgo_table_->setHorizontalHeaderLabels(curgo_table_colum);
    auto curgo_table_lay = new QVBoxLayout();
    curgo_table_lay->addWidget(curgo_table_label);
    curgo_table_lay->addWidget(curgo_table_);

    auto curgo_label = new QLabel("Cargo", this);
    curgo_label->setFont(bold_font);

    auto curgo_code_lay = new QHBoxLayout();
    auto curgo_code_label = new QLabel("Code:", this);
    auto curgo_code_line_edit = new QLineEdit(this);
    curgo_code_lay->addWidget(curgo_code_label);
    curgo_code_lay->setSpacing(5);
    curgo_code_lay->addWidget(curgo_code_line_edit);

    auto curgo_weight_lay = new QHBoxLayout();
    auto curgo_weight_label = new QLabel("Weight:", this);
    auto curgo_weight_line_edit = new QLineEdit(this);
    curgo_weight_lay->addWidget(curgo_weight_label);
    curgo_weight_lay->setSpacing(5);
    curgo_weight_lay->addWidget(curgo_weight_line_edit);

    auto curgo_volume_lay = new QHBoxLayout();
    auto curgo_volume_label = new QLabel("Code:", this);
    auto curgo_volume_line_edit = new QLineEdit(this);
    curgo_volume_lay->addWidget(curgo_volume_label);
    curgo_volume_lay->setSpacing(5);
    curgo_volume_lay->addWidget(curgo_volume_line_edit);

    add_curgo_to_warehouse_button_ = new QPushButton("add new curgo",this);


    auto curgo_add_lay = new QVBoxLayout();
    curgo_add_lay->setAlignment(Qt::AlignTop);
    curgo_add_lay->addWidget(curgo_label);
    curgo_add_lay->setSpacing(5);
    curgo_add_lay->addItem(curgo_code_lay);
    curgo_add_lay->setSpacing(5);
    curgo_add_lay->addItem(curgo_weight_lay);
    curgo_add_lay->setSpacing(5);
    curgo_add_lay->addItem(curgo_volume_lay);
    curgo_add_lay->setSpacing(5);
    curgo_add_lay->addWidget(add_curgo_to_warehouse_button_);

    auto curgo_lay = new QHBoxLayout();
    curgo_lay->addItem(curgo_add_lay);
    curgo_lay->addSpacing(10);
    curgo_lay->addItem(curgo_table_lay);

    create_warehouse_button_ = new QPushButton("Add a new warehouse", this);

    setLayout(new QVBoxLayout);

    layout()->addWidget(warehouse_code_label);
    layout()->addWidget(warehouse_code_);
    layout()->addWidget(warehouse_position_label);
    layout()->addItem(warehouse_position_lay);
    layout()->addItem(curgo_lay);
    layout()->addWidget(create_warehouse_button_);

    connect(create_warehouse_button_, &QAbstractButton::clicked, this, &CreateWarehouseDialog::createWarehouseButtonClicked);

}

void CreateWarehouseDialog::createWarehouseButtonClicked()
{

}
