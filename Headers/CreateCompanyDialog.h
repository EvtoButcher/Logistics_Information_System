#ifndef CREATECOMPANYDIALOG_H
#define CREATECOMPANYDIALOG_H

#include <QDialog>

#include "Company.h"

class QLineEdit;
class WarehouseWidget;
class Company;

class CreateCompanyDialog : public QDialog
{
    Q_OBJECT
    friend Company;
public:
    explicit CreateCompanyDialog(QWidget *parent = nullptr);

    Company *getCompany();

private:
    QLineEdit* company_name_line_edit_;
    WarehouseWidget* warehouse_widget_;
    Company* company_;
};


#endif // CREATECOMPANYDIALOG_H
