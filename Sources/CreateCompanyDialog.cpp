#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFont>

#include "Headers/CreateCompanyDialog.h"
#include "Headers/WarehouseControlWidget.h"
#include "Headers/Company.h"

CreateCompanyDialog::CreateCompanyDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Create Company:");
    setBaseSize(400, 400);

    setLayout(new QVBoxLayout(this));

    auto bold_font = QFont("Ubuntu", 11, QFont::Bold);

    auto company_name_label = new QLabel("Company name", this);
    company_name_label->setFont(bold_font);
    company_name_line_edit_ = new QLineEdit(this);

    warehouse_widget_ = new WarehouseWidget(this);

    layout()->addWidget(company_name_label);
    layout()->addWidget(company_name_line_edit_);
    layout()->addWidget(warehouse_widget_);
}

Company *CreateCompanyDialog::getCompany()
{
    return company_;
}
