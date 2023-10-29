#ifndef DESTINATIONCONTROLWUDGET_H
#define DESTINATIONCONTROLWUDGET_H

#include <QWidget>
#include "Company.h"

class QPushButton;
class QTableWidget;
class QLabel;
class CreateDestinationDialog;
//class Destination;

class DestinationWudget : public QWidget
{
    Q_OBJECT
public:
    explicit DestinationWudget(QWidget* parent = nullptr);

signals:
    void addDestinationToCompany(Destination*);

private slots:
    void addDestination(Destination* destination);

private:
    QLabel*                  destination_label_;
    QLabel*                  destination_table_label_;
    QTableWidget*            destination_table_;
    QPushButton*             add_destination_button_;
    QPushButton*             delete_destination_button_;
    CreateDestinationDialog* create_destination_dialog_;

};
#endif // DESTINATIONCONTROLWUDGET_H
