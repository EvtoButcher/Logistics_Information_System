#ifndef CREATECOMPANYDIALOG_H
#define CREATECOMPANYDIALOG_H

#include <QDialog>

#include "Company.h"

class QLineEdit;
class WarehouseWidget;
class Company;
class QLabel;
class QPushButton;
class WarehouseModel;

class CreateCompanyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CreateCompanyDialog(WarehouseModel& model, QWidget *parent = nullptr);

    Company* getCompany();

signals:
    void addWarehouseOnMap(const uint64_t code, const QGeoCoordinate position);

private slots:
    void addWarehouse(Warehouse* warehouse);
    void trySetCompanyName();
    void createComponyButtonClicked();

private:
    QPushButton*       create_company_button_;
    QPushButton*       close_button_;
    QLabel*            messege_label_;
    QLineEdit*         company_name_line_edit_;
    WarehouseWidget*   warehouse_widget_;
    Company*           company_;
};


#endif // CREATECOMPANYDIALOG_H
