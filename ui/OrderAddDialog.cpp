#include <QVBoxLayout>
#include <QPushButton>

#include "OrderAddDialog.h"
#include "OrderAddWidget.h"


OrderAddDialog::OrderAddDialog(QWidget *parent)
    : QDialog{parent}
{
    setWindowTitle(tr("Create new Order"));
    add_widget_ = new OrderAddWidget(parent);
    close_dialog_button_ = new QPushButton(tr("Close"), this);

    setLayout(new QVBoxLayout);

    layout()->addWidget(add_widget_);
    layout()->addWidget(close_dialog_button_);

    connect(close_dialog_button_, &QAbstractButton::clicked, this, &OrderAddDialog::onCloseButtonClicked);
    connect(add_widget_, &OrderAddWidget::addRoute, this, &OrderAddDialog::onAddWidgetButtonClicked);
}

const OrderAddWidget* OrderAddDialog::getOrderAddWidget()
{
    return add_widget_;
}

void OrderAddDialog::onAddWidgetButtonClicked(const RouteInfo& info)
{
    emit addRouteToTableFromDialog(info);
}

void OrderAddDialog::onCloseButtonClicked()
{
    this->close();
}
