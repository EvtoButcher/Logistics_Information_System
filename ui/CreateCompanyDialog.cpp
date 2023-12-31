#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFont>
#include <QPushButton>
#include <QQuickWidget>
#include <QQmlContext>

#include <QDebug>

#include "CreateCompanyDialog.h"
#include "WarehouseControlWidget.h"
#include "DestinationControlWudget.h"
#include "WarehouseModel.h"
#include "DestinationModel.h"
#include "Company.h"

CreateCompanyDialog::CreateCompanyDialog(DestinationModel& destination_model, WarehouseModel& warehouse_model, QWidget* parent)
    : QDialog(parent)
    , company_(new Company(parent))
    , settings_map_(new QQuickWidget(parent))
{
    setWindowTitle(tr("Create Company"));
    setBaseSize(400, 400);

    setLayout(new QHBoxLayout(this));

    auto bold_font = QFont("Ubuntu", 11, QFont::Bold);

    auto company_name_label = new QLabel(tr("Company name"), this);
    company_name_label->setFont(bold_font);
    company_name_line_edit_ = new QLineEdit(this);

    messege_label_ = new QLabel(tr("enter the company name for further actions"), this);
    messege_label_->setStyleSheet("color: #b7161a;");

    auto separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setFixedHeight(2);

    warehouse_widget_ = new WarehouseWidget(/*model, */this);
    warehouse_widget_->setEnabled(false);

    auto separator_1 = new QFrame(this);
    separator_1->setFrameShape(QFrame::HLine);
    separator_1->setFrameShadow(QFrame::Sunken);
    separator_1->setFixedHeight(2);

    destination_widget_ = new DestinationWudget(this);
    destination_widget_->setEnabled(false);

    create_company_button_ = new QPushButton(tr("Create company"),this);
    create_company_button_->setEnabled(false);
    auto button_lay = new QHBoxLayout();
    button_lay->addWidget(create_company_button_);

    settings_map_->setMinimumSize(QSize(700, 700));
    settings_map_->rootContext()->setContextProperty(CONTEXT_WAREHOUSE_NAME, &warehouse_model);
    settings_map_->rootContext()->setContextProperty(CONTEXT_DESTINATION_NAME, &destination_model);
    settings_map_->setSource(QUrl(QStringLiteral("qrc:/SettingMap.qml")));

    auto control_lay = new QVBoxLayout();

    control_lay->setSpacing(10);
    control_lay->addWidget(company_name_label);
    control_lay->addWidget(company_name_line_edit_);
    control_lay->addWidget(messege_label_);
    control_lay->addWidget(separator);
    control_lay->addWidget(warehouse_widget_);
    control_lay->addWidget(separator_1);
    control_lay->addWidget(destination_widget_);
    control_lay->addItem(button_lay);

    layout()->addItem(control_lay);
    layout()->setSpacing(10);
    layout()->addWidget(settings_map_);

    connect(create_company_button_, &QAbstractButton::clicked, this, &CreateCompanyDialog::createComponyButtonClicked);
    connect(company_name_line_edit_, &QLineEdit::textEdited, this, &CreateCompanyDialog::trySetCompanyName);
    connect(warehouse_widget_, &WarehouseWidget::addWarehouseToCompany, this, &CreateCompanyDialog::addWarehouse);
    connect(destination_widget_, &DestinationWudget::addDestinationToCompany, this, &CreateCompanyDialog::addDestination);
}

Company* CreateCompanyDialog::getCompany()
{
    return company_;
}

void CreateCompanyDialog::addWarehouse(Warehouse* warehouse)
{
    company_->addWarehouse(warehouse);
    emit addWarehouseOnMap(warehouse->getCode(), warehouse->getPosition());
}

void CreateCompanyDialog::addDestination(company_item::Destination* destination)
{
    company_->addDestination(destination);
    emit addDestinationOnMap(destination->getCode(), destination->getPosition());
}

void CreateCompanyDialog::trySetCompanyName()
{
    if(!company_name_line_edit_->text().isEmpty()) {
       messege_label_->hide();
       company_->setName(company_name_line_edit_->text());
       warehouse_widget_->setEnabled(true);
       destination_widget_->setEnabled(true);
       create_company_button_->setEnabled(true);
    }
    else {
        messege_label_->show();
        create_company_button_->setEnabled(false);
        warehouse_widget_->setEnabled(false);
        destination_widget_->setEnabled(false);
    }
}

void CreateCompanyDialog::createComponyButtonClicked()
{
    this->close();
}


