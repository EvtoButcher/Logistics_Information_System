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
#include <QColorDialog>

#include "RouteAddDialog.h"

RouteDialog::RouteDialog(QWidget *parent)
    : QDialog{parent}
{
    auto bold_font = QFont("Ubuntu", 11, QFont::Bold);

    auto route_name_label = new QLabel("Route name:", this);
    route_name_label->setFont(bold_font);
    name_line_edit_ = new QLineEdit("enter the name of the route", this);

    auto color_label = new QLabel("Color", this);
    color_label->setFont(bold_font);
    color_change_button_ = new QPushButton(this);
    color_change_button_->setStyleSheet("background-color: green;");

    color_dialog_ = new QColorDialog(QColor("green"), this);

    auto start_name_label = new QLabel("Start point", this);
    start_name_label->setFont(bold_font);
    auto start_lat_label = new QLabel("latitude:", this);
    start_point_lat_ = new QLineEdit("55.908961", this);
    auto start_lng_label = new QLabel("longitude:", this);
    start_point_lng_ = new QLineEdit("37.391218", this);

    auto stop_name_label = new QLabel("Stop point", this);
    stop_name_label->setFont(bold_font);
    auto stop_lat_label = new QLabel("latitude:", this);
    end_point_lat_ = new QLineEdit("55.9833043", this);
    auto stop_lng_label = new QLabel("longitude:", this);
    end_point_lng_ = new QLineEdit("37.2106466", this);

    add_route_button_ = new QPushButton("Add Route", this);
    close_dialog_button_ = new QPushButton("Close", this);

    setLayout(new QVBoxLayout);

    auto start_coord_lay = new QHBoxLayout();
    start_coord_lay->addWidget(start_lat_label);
    start_coord_lay->addSpacing(10);
    start_coord_lay->addWidget(start_point_lat_);
    start_coord_lay->addSpacing(10);
    start_coord_lay->addWidget(start_lng_label);
    start_coord_lay->addSpacing(10);
    start_coord_lay->addWidget(start_point_lng_);

    auto stop_coord_lay = new QHBoxLayout();
    stop_coord_lay->addWidget(stop_lat_label);
    stop_coord_lay->addSpacing(10);
    stop_coord_lay->addWidget(end_point_lat_);
    stop_coord_lay->addSpacing(10);
    stop_coord_lay->addWidget(stop_lng_label);
    stop_coord_lay->addSpacing(10);
    stop_coord_lay->addWidget(end_point_lng_);

    auto color_lay = new QHBoxLayout();
    color_lay->setAlignment(Qt::AlignLeft);
    color_lay->addWidget(color_label);
    color_lay->setSpacing(10);
    color_lay->addWidget(color_change_button_);

    auto button_lay = new QHBoxLayout();
    button_lay->addWidget(add_route_button_);
    button_lay->addWidget(close_dialog_button_);
    button_lay->setSpacing(10);

    layout()->addWidget(route_name_label);
    layout()->addWidget(name_line_edit_);

    layout()->addItem(color_lay);

    layout()->addWidget(start_name_label);
    layout()->addItem(start_coord_lay);

    layout()->addWidget(stop_name_label);
    layout()->addItem(stop_coord_lay);

    layout()->addItem(button_lay);
    QSpacerItem* verticalSpacer = new QSpacerItem(10, 10 , QSizePolicy::Minimum, QSizePolicy::Expanding);
    layout()->addItem(verticalSpacer);

    connect(close_dialog_button_, &QAbstractButton::clicked, this, &RouteDialog::onCloseButtonClicked);
    connect(add_route_button_, &QAbstractButton::clicked, this, &RouteDialog::onAddButtonClicked);
    connect(color_change_button_, &QAbstractButton::clicked, this, &RouteDialog::onColorChangeButtonClicked);

}

void RouteDialog::onAddButtonClicked()
{
    RouteInfo route_info(name_line_edit_->text(),
                        start_point_lat_->text().toDouble(),
                        start_point_lng_->text().toDouble(),
                        end_point_lat_->text().toDouble(),
                        end_point_lng_->text().toDouble(),
                        color_dialog_->currentColor().name());

    emit addRouteToTable(route_info);
}

void RouteDialog::onColorChangeButtonClicked()
{
    color_dialog_->exec();
    auto color = color_dialog_->currentColor().name();
    color_change_button_->setStyleSheet("background-color: " + color + QString(";"));
    color_changed_ = true;
}

void RouteDialog::onCloseButtonClicked()
{
    this->close();
}
