#include <QSpacerItem>
#include <QSizePolicy>
#include <QFont>
#include <QtDebug>
#include <QColor>
#include <QColorDialog>
#include <QString>
#include <QLineEdit>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QComboBox>
#include <QPair>

#include "OrderAddWidget.h"
#include "Company.h"

OrderAddWidget::OrderAddWidget(QWidget *parent)
    : QWidget(parent)
{
    auto bold_font = QFont("Ubuntu", 11, QFont::Bold);

    auto route_name_label = new QLabel(tr("Route name:"), this);
    route_name_label->setFont(bold_font);
    name_line_edit_ = new QLineEdit(tr("enter the name of the route"), this);

    auto color_label = new QLabel(tr("Color"), this);
    color_label->setFont(bold_font);
    color_change_button_ = new QPushButton(this);
    color_change_button_->setStyleSheet("background-color: green;");

    color_dialog_ = new QColorDialog(QColor("green"), this);

    auto start_position_editing_lay = new QHBoxLayout();
    choose_warehouse_type_ = new QRadioButton(tr("choose a warehouse"), this);
    arbitrary_start_point_type_ = new QRadioButton(tr("arbitrary point"), this);
    arbitrary_start_point_type_->setChecked(true);
    auto start_button_group = new QButtonGroup(this);
    start_button_group->addButton(choose_warehouse_type_);
    start_button_group->addButton(arbitrary_start_point_type_);
    start_position_editing_lay->addWidget(choose_warehouse_type_);
    start_position_editing_lay->addWidget(arbitrary_start_point_type_);

    warehouse_combo_label_ = new QLabel(tr("Start Warehouse"), this);
    warehouse_combo_label_->setFont(bold_font);
    warehouse_combo_label_->hide();
    warehouse_combo_ = new QComboBox(this);
    warehouse_combo_->setInsertPolicy(QComboBox::InsertPolicy::InsertAtTop);
    warehouse_combo_->hide();

    destination_combo_label_ = new QLabel(tr("End Destination"), this);
    destination_combo_label_->setFont(bold_font);
    destination_combo_label_->hide();
    destination_combo_ = new QComboBox(this);
    destination_combo_->setInsertPolicy(QComboBox::InsertPolicy::InsertAtTop);
    destination_combo_->hide();

    start_name_label_ = new QLabel(tr("Start point"), this);
    start_name_label_->setFont(bold_font);
    start_lat_label_ = new QLabel(tr("latitude:"), this);
    start_point_lat_ = new QLineEdit("55.908961", this);
    start_lng_label_ = new QLabel(tr("longitude:"), this);
    start_point_lng_ = new QLineEdit("37.391218", this);

    auto end_position_editing_lay = new QHBoxLayout();
    choose_destination_type_ = new QRadioButton(tr("choose a warehouse"), this);
    arbitrary_end_point_type_ = new QRadioButton(tr("arbitrary point"), this);
    arbitrary_end_point_type_->setChecked(true);
    auto end_button_group = new QButtonGroup(this);
    end_button_group->addButton(choose_destination_type_);
    end_button_group->addButton(arbitrary_end_point_type_);
    end_position_editing_lay->addWidget(choose_destination_type_);
    end_position_editing_lay->addWidget(arbitrary_end_point_type_);

    stop_name_label_ = new QLabel(tr("Stop point"), this);
    stop_name_label_->setFont(bold_font);
    stop_lat_label_ = new QLabel(tr("latitude:"), this);
    end_point_lat_ = new QLineEdit("55.9833043", this);
    stop_lng_label_ = new QLabel(tr("longitude:"), this);
    end_point_lng_ = new QLineEdit("37.2106466", this);

    auto separator_1 = new QFrame(this);
    separator_1->setFrameShape(QFrame::HLine);
    separator_1->setFrameShadow(QFrame::Sunken);
    separator_1->setFixedHeight(2);

    auto separator_2 = new QFrame(this);
    separator_2->setFrameShape(QFrame::HLine);
    separator_2->setFrameShadow(QFrame::Sunken);
    separator_2->setFixedHeight(2);

    add_route_button_ = new QPushButton(tr("Add order"), this);

    setLayout(new QVBoxLayout(this));

    auto start_warehouse_lay = new QHBoxLayout();
    start_warehouse_lay->addWidget(warehouse_combo_label_);
    //start_warehouse_lay->addSpacing(10);
    start_warehouse_lay->addWidget(warehouse_combo_);

    auto end_destination_lay = new QHBoxLayout();
    end_destination_lay->addWidget(destination_combo_label_);
    //start_warehouse_lay->addSpacing(10);
    end_destination_lay->addWidget(destination_combo_);


    auto start_coord_lay = new QHBoxLayout();
    start_coord_lay->addWidget(start_lat_label_);
    start_coord_lay->addSpacing(10);
    start_coord_lay->addWidget(start_point_lat_);
    start_coord_lay->addSpacing(10);
    start_coord_lay->addWidget(start_lng_label_);
    start_coord_lay->addSpacing(10);
    start_coord_lay->addWidget(start_point_lng_);

    auto stop_coord_lay = new QHBoxLayout();
    stop_coord_lay->addWidget(stop_lat_label_);
    stop_coord_lay->addSpacing(10);
    stop_coord_lay->addWidget(end_point_lat_);
    stop_coord_lay->addSpacing(10);
    stop_coord_lay->addWidget(stop_lng_label_);
    stop_coord_lay->addSpacing(10);
    stop_coord_lay->addWidget(end_point_lng_);

    auto color_lay = new QHBoxLayout();
    color_lay->setAlignment(Qt::AlignLeft);
    color_lay->addWidget(color_label);
    color_lay->setSpacing(10);
    color_lay->addWidget(color_change_button_);

    auto button_lay = new QHBoxLayout();
    button_lay->addWidget(add_route_button_);
    button_lay->setSpacing(10);

    layout()->addWidget(route_name_label);
    layout()->addWidget(name_line_edit_);

    layout()->addItem(color_lay);
    layout()->addWidget(separator_2);

    layout()->addItem(start_position_editing_lay);
    layout()->addWidget(start_name_label_);
    layout()->addItem(start_coord_lay);
    layout()->addItem(start_warehouse_lay);
    layout()->setSpacing(10);
    layout()->addWidget(separator_1);

    layout()->addItem(end_position_editing_lay);
    layout()->addWidget(stop_name_label_);
    layout()->addItem(stop_coord_lay);
    layout()->addItem(end_destination_lay);


    QSpacerItem* verticalSpacer = new QSpacerItem(10, 10 , QSizePolicy::Minimum, QSizePolicy::Expanding);
    layout()->addItem(verticalSpacer);
    layout()->addItem(button_lay);

    connect(choose_warehouse_type_, &QAbstractButton::clicked, this, &OrderAddWidget::onWarehouseEditType);
    connect(arbitrary_start_point_type_, &QAbstractButton::clicked, this, &OrderAddWidget::onStartPointType);
    connect(choose_destination_type_, &QAbstractButton::clicked, this, &OrderAddWidget::onDestinationEditType);
    connect(arbitrary_end_point_type_, &QAbstractButton::clicked, this, &OrderAddWidget::onEndPointType);

    connect(add_route_button_, &QAbstractButton::clicked, this, &OrderAddWidget::onAddButtonClicked);
    connect(color_change_button_, &QAbstractButton::clicked, this, &OrderAddWidget::onColorChangeButtonClicked);
}

void OrderAddWidget::addNewWarehouseVariant(Warehouse* warehouse)
{
    warehouse_combo_->addItem(QString::number(warehouse->getCode()), QVariant::fromValue(warehouse->getPosition()));
}

void OrderAddWidget::addNewDestinationVariant(Destination* destination)
{
   destination_combo_->addItem(QString::number(destination->getCode()), QVariant::fromValue(destination->getPosition()));
}


void OrderAddWidget::onAddButtonClicked()
{
    RouteInfo route_info;
    route_info.code_ = name_line_edit_->text();

    if(choose_destination_type_->isChecked()) {
        route_info.end_route_point_ = qvariant_cast<QGeoCoordinate>(destination_combo_->currentData());
    }
    else {
        route_info.end_route_point_ = QGeoCoordinate(end_point_lat_->text().toDouble(), end_point_lng_->text().toDouble());
    }

    if(choose_warehouse_type_->isChecked()) {
        route_info.start_route_point_ = qvariant_cast<QGeoCoordinate>(warehouse_combo_->currentData());
    }
    else {
        route_info.start_route_point_ = QGeoCoordinate(start_point_lat_->text().toDouble(), start_point_lng_->text().toDouble());
    }

    route_info.route_color_ = color_dialog_->currentColor().name();


//    RouteInfo route_info(name_line_edit_->text(),
//                        start_point_lat_->text().toDouble(),
//                        start_point_lng_->text().toDouble(),
//                        end_point_lat_->text().toDouble(),
//                        end_point_lng_->text().toDouble(),
//                        color_dialog_->currentColor().name());

    qDebug() << route_info.end_route_point_;
    qDebug() << route_info.start_route_point_;

    emit addRoute(route_info);
}

void OrderAddWidget::onColorChangeButtonClicked()
{
    color_dialog_->exec();
    auto color = color_dialog_->currentColor().name();
    color_change_button_->setStyleSheet("background-color: " + color + QString(";"));
}

void OrderAddWidget::onCloseButtonClicked()
{
    this->close();
}

void OrderAddWidget::onWarehouseEditType()
{
    start_name_label_->hide();
    start_lat_label_->hide();
    start_point_lat_->hide();
    start_lng_label_->hide();
    start_point_lng_->hide();

    warehouse_combo_label_->show();
    warehouse_combo_->show();
}

void OrderAddWidget::onStartPointType()
{
    start_name_label_->show();
    start_lat_label_->show();
    start_point_lat_->show();
    start_lng_label_->show();
    start_point_lng_->show();

    warehouse_combo_label_->hide();
    warehouse_combo_->hide();
}

void OrderAddWidget::onDestinationEditType()
{
    stop_name_label_->hide();
    stop_lat_label_->hide();
    end_point_lat_->hide();
    stop_lng_label_->hide();
    end_point_lng_->hide();

    destination_combo_label_->show();
    destination_combo_->show();
}

void OrderAddWidget::onEndPointType()
{
    stop_name_label_->show();
    stop_lat_label_->show();
    end_point_lat_->show();
    stop_lng_label_->show();
    end_point_lng_->show();

    destination_combo_label_->hide();
    destination_combo_->hide();
}
