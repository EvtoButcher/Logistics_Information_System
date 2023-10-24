#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFont>
#include <QPushButton>

#include "Headers/CreateCompanyDialog.h"
#include "Headers/WarehouseControlWidget.h"
#include "Headers/Company.h"

CreateCompanyDialog::CreateCompanyDialog(QWidget *parent)
    : QDialog(parent)
    , company_(new Company)
{
    setWindowTitle("Create Company:");
    setBaseSize(400, 400);

    setLayout(new QVBoxLayout(this));

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

    warehouse_widget_ = new WarehouseWidget(this);
    warehouse_widget_->setEnabled(false);


    create_company_button_ = new QPushButton("Create company",this);
    create_company_button_->setEnabled(false);
    //close_button_ = new QPushButton("Close",this);
    auto button_lay = new QHBoxLayout();
    button_lay->addWidget(create_company_button_);
    //button_lay->addSpacing(10);
    //button_lay->addWidget(close_button_);


    layout()->setSpacing(10);
    layout()->addWidget(company_name_label);
    layout()->addWidget(company_name_line_edit_);
    layout()->addWidget(messege_label_);
    layout()->addWidget(separator);
    layout()->addWidget(warehouse_widget_);
    layout()->addItem(button_lay);

    //connect(close_button_, &QAbstractButton::clicked, this, &CreateCompanyDialog::close);
    connect(company_name_line_edit_, &QLineEdit::textEdited, this, &CreateCompanyDialog::trySetCompanyName);
    connect(warehouse_widget_, &WarehouseWidget::addWarehouse, this, &CreateCompanyDialog::addWarehouse);
}

Company* CreateCompanyDialog::getCompany()
{
    return company_;
}

void CreateCompanyDialog::addWarehouse(Warehouse* warehouse)
{
    company_->addWarehouse(warehouse);
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

