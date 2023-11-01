#ifndef CREATECOMPANYDIALOG_H
#define CREATECOMPANYDIALOG_H

#include <QDialog>

#include "Company.h"

class QLineEdit;
class WarehouseWidget;
class DestinationWudget;
class Company;
class QLabel;
class QPushButton;
class WarehouseModel;
class QQuickWidget;
class DestinationModel;

class CreateCompanyDialog : public QDialog
{   
    Q_OBJECT
public:
    explicit CreateCompanyDialog(DestinationModel& destination_model, WarehouseModel& warehouse_model, QWidget *parent = nullptr);

    Company* getCompany();

signals:
    void addWarehouseOnMap(const uint64_t code, const QGeoCoordinate position);
    void addDestinationOnMap(const uint64_t code, const QGeoCoordinate position);

private slots:
    void addWarehouse(Warehouse* warehouse);
    void addDestination(Destination* destination);
    void trySetCompanyName();
    void createComponyButtonClicked();

private:
    QPushButton*       create_company_button_;
    QPushButton*       close_button_;
    QLabel*            messege_label_;
    QLineEdit*         company_name_line_edit_;
    WarehouseWidget*   warehouse_widget_;
    DestinationWudget* destination_widget_;
    Company*           company_;

    QQuickWidget*      settings_map_;
};


#endif // CREATECOMPANYDIALOG_H
