#ifndef DESTINATIONCONTROLWUDGET_H
#define DESTINATIONCONTROLWUDGET_H

#include <QWidget>

#include "AbstractCompanyItemModel.h"

class QPushButton;
class QTableWidget;
class QLabel;
class CreateDestinationDialog;

class DestinationWudget : public QWidget
{
    Q_OBJECT
public:
    explicit DestinationWudget(QWidget* parent = nullptr);

signals:
    void addDestinationToCompany(company_item::Destination*);
    void removeDestinationFromCompany(const uint64_t code, const int row);

private slots:
    void addDestination(company_item::Destination* destination);
    void deleteDestinationButtonClicked();

private:
    QLabel*                  destination_label_;
    QLabel*                  destination_table_label_;
    QTableWidget*            destination_table_;
    QPushButton*             add_destination_button_;
    QPushButton*             delete_destination_button_;
    CreateDestinationDialog* create_destination_dialog_;

};
#endif // DESTINATIONCONTROLWUDGET_H
