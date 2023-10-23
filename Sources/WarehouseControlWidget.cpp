#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFont>
#include <QTableWidget>
#include <QStringList>
#include <QPushButton>

#include <QDebug>

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
    cargo_from_warehouses_table_colum << "code" << "Weight" << "Volume";
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

    connect(add_warehouse_button_, &QAbstractButton::clicked, this, &WarehouseWidget::addNewWarehouse);
}

void WarehouseWidget::addNewWarehouse()
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
    warehouse_code_ = new QLineEdit("automatic generation", this);
    warehouse_code_->setEnabled(false);

    auto warehouse_position_label = new QLabel("Position:", this);
    warehouse_position_label->setFont(bold_font);
    auto warehouse_lat_label = new QLabel("latitude:", this);
    warehouse_lat_ = new QLineEdit(this);
    auto warehouse_lng_label = new QLabel("longitude:", this);
    warehouse_lng_ = new QLineEdit(this);

    messege_label = new QLabel("enter the coordinates of the warehouse for further actions", this);
    messege_label->setStyleSheet("color: #b7161a;");

    auto separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setFixedHeight(2);

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
    curgo_table_colum << "code" << "Weight" << "Volume";
    curgo_table_->setHorizontalHeaderLabels(curgo_table_colum);
    curgo_table_->setEnabled(false);
    auto curgo_table_lay = new QVBoxLayout();
    curgo_table_lay->addWidget(curgo_table_label);
    curgo_table_lay->addWidget(curgo_table_);

    auto curgo_label = new QLabel("Cargo", this);
    curgo_label->setFont(bold_font);

    auto curgo_code_lay = new QHBoxLayout();
    auto curgo_code_label = new QLabel("Code:", this);
    curgo_code_line_edit_ = new QLineEdit(this);
    curgo_code_line_edit_->setEnabled(false);
    curgo_code_lay->addWidget(curgo_code_label);
    curgo_code_lay->setSpacing(5);
    curgo_code_lay->addWidget(curgo_code_line_edit_);

    auto curgo_weight_lay = new QHBoxLayout();
    auto curgo_weight_label = new QLabel("Weight:", this);
    curgo_weight_line_edit_ = new QLineEdit(this);
    curgo_weight_line_edit_->setEnabled(false);
    curgo_weight_lay->addWidget(curgo_weight_label);
    curgo_weight_lay->setSpacing(5);
    curgo_weight_lay->addWidget(curgo_weight_line_edit_);

    auto curgo_volume_lay = new QHBoxLayout();
    auto curgo_volume_label = new QLabel("Volume:", this);
    curgo_volume_line_edit_ = new QLineEdit(this);
    curgo_volume_line_edit_->setEnabled(false);
    curgo_volume_lay->addWidget(curgo_volume_label);
    curgo_volume_lay->setSpacing(5);
    curgo_volume_lay->addWidget(curgo_volume_line_edit_);

    add_curgo_to_warehouse_button_ = new QPushButton("add new curgo",this);
    add_curgo_to_warehouse_button_->setEnabled(false);


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
    create_warehouse_button_->setEnabled(false);

    setLayout(new QVBoxLayout);

    layout()->addWidget(warehouse_code_label);
    layout()->addWidget(warehouse_code_);
    layout()->addWidget(warehouse_position_label);
    layout()->addItem(warehouse_position_lay);
    layout()->setSpacing(10);
    layout()->addWidget(messege_label);
    layout()->addWidget(separator);
    layout()->setSpacing(10);
    layout()->addItem(curgo_lay);
    layout()->addWidget(create_warehouse_button_);

    connect(add_curgo_to_warehouse_button_, &QAbstractButton::clicked, this, &CreateWarehouseDialog::addCurgo);
    connect(create_warehouse_button_, &QAbstractButton::clicked, this, &CreateWarehouseDialog::createWarehouseButtonClicked);
    connect(warehouse_lat_, &QLineEdit::textEdited, this, &CreateWarehouseDialog::warehouseLatAdding);
    connect(warehouse_lng_, &QLineEdit::textEdited, this, &CreateWarehouseDialog::warehouseLngAdding);
    connect(warehouse_lat_, &QLineEdit::editingFinished, this, &CreateWarehouseDialog::tryCreateWarehouse);
    connect(warehouse_lng_, &QLineEdit::editingFinished, this, &CreateWarehouseDialog::tryCreateWarehouse);

}

void CreateWarehouseDialog::createWarehouseButtonClicked()
{

}

void CreateWarehouseDialog::tryCreateWarehouse()
{
    if(warehouse_lat_->text().isEmpty() || warehouse_lng_->text().isEmpty()){
        return;
    }

    auto warehouse_position = QGeoCoordinate(warehouse_lat_->text().toDouble(), warehouse_lng_->text().toDouble());
    if(!warehouse_position.isValid()){
        return;
    }

    warehouse_ = new Warehouse(warehouse_position);
    warehouse_code_->setText(QString::number(warehouse_->getCode()));

    warehouse_code_->setStyleSheet("color: #91918b;");

    //TODO: add hendeling correct creating warehouse

    curgo_table_->setEnabled(true);
    curgo_code_line_edit_->setEnabled(true);
    curgo_weight_line_edit_->setEnabled(true);
    curgo_volume_line_edit_->setEnabled(true);
    add_curgo_to_warehouse_button_->setEnabled(true);

    create_warehouse_button_->setEnabled(true);
}

void CreateWarehouseDialog::addCurgo()
{

    warehouse_->addCurgo(new Curgo(curgo_code_line_edit_->text(),
                               curgo_weight_line_edit_->text().toDouble(),
                               curgo_volume_line_edit_->text().toInt()));

    //QTableWidgetItem *newItem = new QTableWidgetItem("", 1, 2);
    //curgo_table_->currentItem() = newItem;
}

void CreateWarehouseDialog::warehouseLatAdding()
{
    bool convert_is_correct;
    warehouse_lat_->text().toDouble(&convert_is_correct);
    if(!convert_is_correct) {
        warehouse_lat_->setStyleSheet("color: #b7161a;");
    }
    else {
         warehouse_lat_->setStyleSheet("color: black;");
    }
}

void CreateWarehouseDialog::warehouseLngAdding()
{
    bool convert_is_correct;
    warehouse_lng_->text().toDouble(&convert_is_correct);
    if(!convert_is_correct) {
        warehouse_lng_->setStyleSheet("color: #b7161a;");
    }
    else {
         warehouse_lng_->setStyleSheet("color: black;");
    }
}

