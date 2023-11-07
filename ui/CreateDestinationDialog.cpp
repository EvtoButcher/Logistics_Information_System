#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGeoCoordinate>

#include <QDebug>

#include "CreateDestinationDialog.h"
#include "Company.h"

CreateDestinationDialog::CreateDestinationDialog(QWidget *parent)
    : QDialog(parent)
{
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setWindowTitle(tr("Create Warehouse"));

    auto bold_font = QFont("Ubuntu", 11, QFont::Bold);
    auto destination_code_label = new QLabel(tr("Destination Code"), this);
    destination_code_label->setFont(bold_font);

    destination_code_ = new QLineEdit(tr("automatic generation"), this);
    destination_code_->setEnabled(false);

    auto destination_code_lay = new QHBoxLayout();
    destination_code_lay->addWidget(destination_code_);
    auto spacer = new QSpacerItem(250, 0, QSizePolicy::Fixed);
    destination_code_lay->addItem(spacer);

    auto destination_position_label = new QLabel(tr("Position:"), this);
    destination_position_label->setFont(bold_font);
    auto destination_lat_label = new QLabel(tr("latitude:"), this);
    destination_lat_ = new QLineEdit(this);
    auto destination_lng_label = new QLabel(tr("longitude:"), this);
    destination_lng_ = new QLineEdit(this);

    auto destination_position_lay = new QHBoxLayout();
    destination_position_lay->addWidget(destination_lat_label);
    destination_position_lay->addSpacing(10);
    destination_position_lay->addWidget(destination_lat_);
    destination_position_lay->addSpacing(10);
    destination_position_lay->addWidget(destination_lng_label);
    destination_position_lay->addSpacing(10);
    destination_position_lay->addWidget(destination_lng_);

    create_destination_button_ = new QPushButton(tr("Add a new destination"), this);

    setLayout(new QVBoxLayout(this));

    layout()->addWidget(destination_code_label);
    layout()->addItem(destination_code_lay);
    layout()->addWidget(destination_position_label);
    layout()->addItem(destination_position_lay);
    layout()->addWidget(create_destination_button_);
    layout()->setSpacing(10);

    connect(create_destination_button_, &QAbstractButton::clicked, this, &CreateDestinationDialog::createDestinationButtonClicked);
    connect(destination_lat_, &QLineEdit::textEdited, this, &CreateDestinationDialog::destinationLatAdding);
    connect(destination_lng_, &QLineEdit::textEdited, this, &CreateDestinationDialog::destinationLngAdding);
}

void CreateDestinationDialog::tryCreateDestination()
{
    if(destination_lat_->text().isEmpty() || destination_lng_->text().isEmpty()){
        return;
    }

    auto destination_position = QGeoCoordinate(destination_lat_->text().toDouble(), destination_lng_->text().toDouble());
    if(!destination_position.isValid()){
        return;
    }

    destination_ = new Destination(destination_position);
    destination_code_->setText(QString::number(destination_->getCode()));
    destination_code_->setStyleSheet("color: #91918b;");

    //TODO: add hendeling correct creating warehouse
}

void CreateDestinationDialog::destinationLatAdding()
{
    bool convert_is_correct;
    destination_lat_->text().toDouble(&convert_is_correct);
    if(!convert_is_correct) {
        destination_lat_->setStyleSheet("color: #b7161a;");
        if(destination_code_->text() != tr("automatic generation")) {
            destination_code_->setText(tr("automatic generation"));
        }
    }
    else {
        destination_lat_->setStyleSheet("color: black;");
        tryCreateDestination();
    }
}

void CreateDestinationDialog::destinationLngAdding()
{
    bool convert_is_correct;
    destination_lng_->text().toDouble(&convert_is_correct);
    if(!convert_is_correct) {
        destination_lng_->setStyleSheet("color: #b7161a;");
        if(destination_code_->text() != tr("automatic generation")) {
            destination_code_->setText(tr("automatic generation"));
        }
    }
    else {
        destination_lng_->setStyleSheet("color: black;");
        tryCreateDestination();
    }
}

void CreateDestinationDialog::createDestinationButtonClicked()
{
    emit createDestination(destination_);
    destination_lat_->clear();
    destination_lng_->clear();
    this->close();
}
