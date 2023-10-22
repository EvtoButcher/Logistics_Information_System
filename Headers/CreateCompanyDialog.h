#ifndef CREATECOMPANYDIALOG_H
#define CREATECOMPANYDIALOG_H

#include <QDialog>

class QLineEdit;
class WarehouseWidget;

class CreateCompanyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateCompanyDialog(QWidget *parent = nullptr);


private:
    QLineEdit* company_name_line_edit_;
    WarehouseWidget* warehouse_widget_;
};


#endif // CREATECOMPANYDIALOG_H
