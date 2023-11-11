#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QGeoCoordinate>
#include <QQuickWidget>
#include <QQmlContext>

#include "CreateWarehouseDialog.h"
#include "Company.h"
#include "WarehouseModel.h"

CreateWarehouseDialog::CreateWarehouseDialog(QWidget* parent)
    : QDialog(parent)
    //, settings_map_(new QQuickWidget(parent))
{
    //settings_map_->rootContext()->setContextProperty(CONTEXT_WAREHOUSE_NAME,
                                                     //map->rootContext()->contextProperty(CONTEXT_WAREHOUSE_NAME).value<QObject*>());
    //settings_map_->setSource(map->source());
    //settings_map_->setMinimumSize(map->minimumSize());
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //this->setFixedSize(700, 500);
    setWindowTitle(tr("Create Warehouse"));

    auto bold_font = QFont("Ubuntu", 11, QFont::Bold);

    auto warehouse_code_label = new QLabel(tr("Warehouse code:"), this);
    warehouse_code_label->setFont(bold_font);
    warehouse_code_ = new QLineEdit(tr("automatic generation"), this);
    warehouse_code_->setEnabled(false);

    auto warehouse_code_lay = new QHBoxLayout();
    warehouse_code_lay->addWidget(warehouse_code_);
    auto spacer = new QSpacerItem(250, 0, QSizePolicy::Fixed);
    warehouse_code_lay->addItem(spacer);

    auto warehouse_position_label = new QLabel(tr("Position:"), this);
    warehouse_position_label->setFont(bold_font);
    auto warehouse_lat_label = new QLabel(tr("latitude:"), this);
    warehouse_lat_ = new QLineEdit(this);
    auto warehouse_lng_label = new QLabel(tr("longitude:"), this);
    warehouse_lng_ = new QLineEdit(this);

    messege_label_ = new QLabel(tr("enter the coordinates of the warehouse for further actions"), this);
    messege_label_->setStyleSheet("color: #b7161a;");

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


    curgo_table_label_ = new QLabel(tr("Added Cargo"), this);
    curgo_table_label_->setEnabled(false);
    curgo_table_ = new QTableWidget(0, 3, this);
    QStringList curgo_table_colum;
    curgo_table_colum << tr("Code") << tr("Weight") << tr("Volume");
    curgo_table_->setHorizontalHeaderLabels(curgo_table_colum);
    curgo_table_->setEnabled(false);
    auto curgo_table_lay = new QVBoxLayout();
    curgo_table_lay->addWidget(curgo_table_label_);
    curgo_table_lay->addWidget(curgo_table_);

    auto curgo_label = new QLabel(tr("Cargo"), this);
    curgo_label->setFont(bold_font);

    auto curgo_weight_lay = new QHBoxLayout();
    curgo_weight_label_ = new QLabel(tr("Weight:"), this);
    curgo_weight_label_->setEnabled(false);
    curgo_weight_line_edit_ = new QLineEdit(this);
    curgo_weight_line_edit_->setEnabled(false);
    curgo_weight_lay->addWidget(curgo_weight_label_);
    curgo_weight_lay->addSpacing(6);
    curgo_weight_lay->addWidget(curgo_weight_line_edit_);

    auto curgo_volume_lay = new QHBoxLayout();
    curgo_volume_label_ = new QLabel(tr("Volume:"), this);
    curgo_volume_label_->setEnabled(false);
    curgo_volume_line_edit_ = new QLineEdit(this);
    curgo_volume_line_edit_->setEnabled(false);
    curgo_volume_lay->addWidget(curgo_volume_label_);
    curgo_volume_lay->setSpacing(3);
    curgo_volume_lay->addWidget(curgo_volume_line_edit_);

    add_curgo_to_warehouse_button_ = new QPushButton(tr("add new curgo"),this);
    add_curgo_to_warehouse_button_->setEnabled(false);


    auto curgo_add_lay = new QVBoxLayout();
    curgo_add_lay->setAlignment(Qt::AlignTop);
    curgo_add_lay->addWidget(curgo_label);
    curgo_add_lay->addSpacing(5);
    curgo_add_lay->addItem(curgo_weight_lay);
    curgo_add_lay->addSpacing(5);
    curgo_add_lay->addItem(curgo_volume_lay);
    curgo_add_lay->addSpacing(10);
    curgo_add_lay->addWidget(add_curgo_to_warehouse_button_);

    auto curgo_lay = new QHBoxLayout();
    curgo_lay->addItem(curgo_add_lay);
    curgo_lay->addSpacing(10);
    curgo_lay->addItem(curgo_table_lay);

    create_warehouse_button_ = new QPushButton(tr("Add a new warehouse"), this);
    create_warehouse_button_->setEnabled(false);

    setLayout(new QHBoxLayout);

    auto control_lay = new QVBoxLayout();

    control_lay->addWidget(warehouse_code_label);
    control_lay->addItem(warehouse_code_lay);
    control_lay->addWidget(warehouse_position_label);
    control_lay->addItem(warehouse_position_lay);
    control_lay->setSpacing(10);
    control_lay->addWidget(messege_label_);
    control_lay->addWidget(separator);
    control_lay->setSpacing(10);
    control_lay->addItem(curgo_lay);
    control_lay->addWidget(create_warehouse_button_);

    layout()->addItem(control_lay);
    //layout()->setSpacing(10);
    //layout()->addWidget(settings_map_);

    connect(add_curgo_to_warehouse_button_, &QAbstractButton::clicked, this, &CreateWarehouseDialog::addCurgo);
    connect(create_warehouse_button_, &QAbstractButton::clicked, this, &CreateWarehouseDialog::createWarehouseButtonClicked);
    connect(warehouse_lat_, &QLineEdit::textEdited, this, &CreateWarehouseDialog::warehouseLatAdding);
    connect(warehouse_lng_, &QLineEdit::textEdited, this, &CreateWarehouseDialog::warehouseLngAdding);
}

void CreateWarehouseDialog::createWarehouseButtonClicked()
{
    emit createWarehouse(warehouse_);
    curgo_table_->setRowCount(0);
    curgo_volume_line_edit_->clear();
    curgo_weight_line_edit_->clear();
    warehouse_lat_->clear();
    warehouse_lng_->clear();
    enabledAddCurgo(false);
    this->close();
}

void CreateWarehouseDialog::enabledAddCurgo(bool status)
{
    curgo_table_->setEnabled(status);
    curgo_weight_line_edit_->setEnabled(status);
    curgo_volume_line_edit_->setEnabled(status);
    add_curgo_to_warehouse_button_->setEnabled(status);
    curgo_table_label_->setEnabled(status);
    curgo_weight_label_->setEnabled(status);
    curgo_volume_label_->setEnabled(status);
    create_warehouse_button_->setEnabled(status);
}

void CreateWarehouseDialog::tryCreateWarehouse()
{
    if(warehouse_lat_->text().isEmpty() || warehouse_lng_->text().isEmpty()){
        return;
    }

    auto warehouse_position = QGeoCoordinate(warehouse_lat_->text().toDouble(), warehouse_lng_->text().toDouble());
    if(!warehouse_position.isValid()){
        enabledAddCurgo(false);
        messege_label_->show();
        return;
    }

    warehouse_ = new Warehouse(warehouse_position);
    warehouse_code_->setText(QString::number(warehouse_->getCode()));

    warehouse_code_->setStyleSheet("color: #91918b;");

    //TODO: add hendeling correct creating warehouse

    enabledAddCurgo(true);
    messege_label_->hide();
}

void CreateWarehouseDialog::addCurgo()
{
    auto current_curgo = new Cargo(curgo_weight_line_edit_->text().toDouble(), curgo_volume_line_edit_->text().toInt());
    warehouse_->addCargo(current_curgo);

    int row_count = curgo_table_->rowCount();
    curgo_table_->setRowCount(row_count + 1);

    for(int colum_index = 0; colum_index < curgo_table_->columnCount(); ++colum_index){
        auto* newItem = new QTableWidgetItem();
        switch (colum_index) {
            case 0:
                newItem->setText("#" + QString::number(current_curgo->code_)); /*Code*/
                break;
            case 1:
                newItem->setText(curgo_weight_line_edit_->text() + tr(" kg")); /*Weight*/
                break;
            case 2:
                newItem->setText(curgo_volume_line_edit_->text() + tr(" сm³")); /*Volume*/
                break;
        }
        curgo_table_->setItem(row_count, colum_index, newItem);
    }
    curgo_table_->resizeColumnToContents(1);
    curgo_table_->resizeColumnToContents(2);

    curgo_weight_line_edit_->clear();
    curgo_volume_line_edit_->clear();
}

void CreateWarehouseDialog::warehouseLatAdding()
{
    bool convert_is_correct;
    warehouse_lat_->text().toDouble(&convert_is_correct);
    if(!convert_is_correct) {
        warehouse_lat_->setStyleSheet("color: #b7161a;");
        enabledAddCurgo(false);
        messege_label_->show();
        if(warehouse_code_->text() != tr("automatic generation")) {
            warehouse_code_->setText(tr("automatic generation"));
        }
    }
    else {
        warehouse_lat_->setStyleSheet("color: black;");
        tryCreateWarehouse();
    }
}

void CreateWarehouseDialog::warehouseLngAdding()
{
    bool convert_is_correct;
    warehouse_lng_->text().toDouble(&convert_is_correct);
    if(!convert_is_correct) {
        warehouse_lng_->setStyleSheet("color: #b7161a;");
        enabledAddCurgo(false);
        messege_label_->show();
        if(warehouse_code_->text() != tr("automatic generation")){
            warehouse_code_->setText(tr("automatic generation"));
        }
    }
    else {
         warehouse_lng_->setStyleSheet("color: black;");
         tryCreateWarehouse();
    }
}
