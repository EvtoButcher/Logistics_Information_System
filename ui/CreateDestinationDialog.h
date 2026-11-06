#ifndef CREATEDESTINATIONDIALOG_H
#define CREATEDESTINATIONDIALOG_H

#include <QDialog>

#include "AbstractCompanyItemModel.h"

class QLabel;
class QLineEdit;
class QTableWidget;
class QPushButton;
class QQuickWidget;
class AbstractCompanyItem;

class CreateDestinationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CreateDestinationDialog(QWidget* parent = nullptr);
    ~CreateDestinationDialog() override;

signals:
    void createDestination(company_item::Destination*);

private slots:
    void tryCreateDestination();
    void destinationLatAdding();
    void destinationLngAdding();
    void createDestinationButtonClicked();

private:
    void enabledAddDestination(bool status);

private:
    company_item::Destination* destination_ = nullptr;

    QLineEdit*      destination_code_;
    QLineEdit*      destination_lat_;
    QLineEdit*      destination_lng_;
    QPushButton*    create_destination_button_;
};
#endif // CREATEDESTINATIONDIALOG_H
