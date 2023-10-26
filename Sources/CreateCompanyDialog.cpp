#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFont>
#include <QPushButton>

#include <QDebug>

#include "Headers/CreateCompanyDialog.h"
#include "Headers/WarehouseControlWidget.h"
#include "Headers/Company.h"

CreateCompanyDialog::CreateCompanyDialog(WarehouseModel& model, QWidget *parent)
    : QDialog(parent)
    , company_(new Company)
{
    setWindowTitle("Create Company");
    setBaseSize(400, 400);

    setLayout(new QHBoxLayout(this));

    auto bold_font = QFont("Ubuntu", 11, QFont::Bold);

    auto company_name_label = new QLabel("Company name", this);
    company_name_label->setFont(bold_font);
    company_name_line_edit_ = new QLineEdit(this);

    messege_label_ = new QLabel("enter the company name for further actions", this);
    messege_label_->setStyleSheet("color: #b7161a;");

    auto separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setFixedHeight(2);

    warehouse_widget_ = new WarehouseWidget(model, this);
    warehouse_widget_->setEnabled(false);

    create_company_button_ = new QPushButton("Create company",this);
    create_company_button_->setEnabled(false);
    //close_button_ = new QPushButton("Close",this);
    auto button_lay = new QHBoxLayout();
    button_lay->addWidget(create_company_button_);
    //button_lay->addSpacing(10);
    //button_lay->addWidget(close_button_);

    auto control_lay = new QVBoxLayout();

    control_lay->setSpacing(10);
    control_lay->addWidget(company_name_label);
    control_lay->addWidget(company_name_line_edit_);
    control_lay->addWidget(messege_label_);
    control_lay->addWidget(separator);
    control_lay->addWidget(warehouse_widget_);
    control_lay->addItem(button_lay);

    layout()->addItem(control_lay);
    layout()->setSpacing(10);
    layout()->addWidget(warehouse_widget_->getSettingsMap());

    //connect(close_button_, &QAbstractButton::clicked, this, &CreateCompanyDialog::close);
    connect(create_company_button_, &QAbstractButton::clicked, this, &CreateCompanyDialog::createComponyButtonClicked);
    connect(company_name_line_edit_, &QLineEdit::textEdited, this, &CreateCompanyDialog::trySetCompanyName);
    connect(warehouse_widget_, &WarehouseWidget::addWarehouseToCompany, this, &CreateCompanyDialog::addWarehouse);
}

Company* CreateCompanyDialog::getCompany()
{
    return company_;
}

void CreateCompanyDialog::addWarehouse(Warehouse* warehouse)
{
    company_->addWarehouse(warehouse);
    qDebug() << warehouse->getCode() <<  warehouse->getPosition();
    emit addWarehouseOnMap(warehouse->getCode(), warehouse->getPosition());
}

void CreateCompanyDialog::trySetCompanyName()
{
    company_->setName(company_name_line_edit_->text());
    warehouse_widget_->setEnabled(true);
    if(!company_name_line_edit_->text().isEmpty()){
       messege_label_->hide();
       create_company_button_->setEnabled(true);
    }
    else {
        messege_label_->show();
        create_company_button_->setEnabled(false);
    }
}

void CreateCompanyDialog::createComponyButtonClicked()
{
    this->close();
}


