#include <QVBoxLayout>
#include <QPushButton>

#include "Headers/OrderAddDialog.h"
#include "Headers/OrderAddWidget.h"


OrderAddDialog::OrderAddDialog(QWidget *parent)
    : QDialog{parent}
{
    add_widget_ = new OrderAddWidget(parent);
    close_dialog_button_ = new QPushButton("Close", this);

    setLayout(new QVBoxLayout);

    layout()->addWidget(add_widget_);
    layout()->addWidget(close_dialog_button_);

    connect(close_dialog_button_, &QAbstractButton::clicked, this, &OrderAddDialog::onCloseButtonClicked);
    connect(add_widget_, &OrderAddWidget::addRouteToTable, this, &OrderAddDialog::onAddWidgetButtonClicked);
}

void OrderAddDialog::onAddWidgetButtonClicked(const RouteInfo& info)
{
    emit addRouteToTableFromDialog(info);
}

void OrderAddDialog::onCloseButtonClicked()
{
    this->close();
}
