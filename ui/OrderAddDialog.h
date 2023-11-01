#ifndef ORDERADDDIALOG_H
#define ORDERADDDIALOG_H

#include <QDialog>

#include "RouteModel.h"

class QPushButton;
class OrderAddWidget;

class OrderAddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OrderAddDialog(QWidget *parent = nullptr);
    ~OrderAddDialog() = default;

    const OrderAddWidget* getOrderAddWidget();

signals:
     void addRouteToTableFromDialog(const RouteInfo&);

private slots:
    void onAddWidgetButtonClicked(const RouteInfo &info);
    void onCloseButtonClicked();

private:
    OrderAddWidget* add_widget_;
    QPushButton*    close_dialog_button_;

};

#endif // ORDERADDDIALOG_H
