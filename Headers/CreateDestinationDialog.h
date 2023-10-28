#ifndef CREATEDESTINATIONDIALOG_H
#define CREATEDESTINATIONDIALOG_H
#include <QDialog>

class QLabel;
class QLineEdit;
class QTableWidget;
class QPushButton;
class QQuickWidget;
class Destination;

class CreateDestinationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CreateDestinationDialog(/*QQuickWidget* map,*/QWidget* parent = nullptr);

signals:
    void createDestination(Destination*);

private slots:
    void tryCreateDestination();
    void destinationLatAdding();
    void destinationLngAdding();

private:
    void enabledAddDestination(bool status);

private:
    Destination* destination_;

    QLineEdit*      destination_code_;
    QLineEdit*      destination_lat_;
    QLineEdit*      destination_lng_;
    QPushButton*    create_destination_button_;
};
#endif // CREATEDESTINATIONDIALOG_H
