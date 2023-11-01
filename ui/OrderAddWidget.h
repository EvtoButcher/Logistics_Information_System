#ifndef ORDERADDWIDGET_H
#define ORDERADDWIDGET_H

#include <QWidget>
#include <QAbstractListModel>

#include "RouteModel.h"

class QLineEdit;
class QDockWidget;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QPushButton;
class QColorDialog;
class QRadioButton;
class QComboBox;
class Warehouse;
class Destination;


class OrderAddWidget: public QWidget
{
    Q_OBJECT

public:
    explicit OrderAddWidget(QWidget *parent = nullptr);
    ~OrderAddWidget() = default;

signals:
     void addRoute(const RouteInfo&);

public slots:
     void addNewWarehouseVariant(Warehouse* warehouse);
     void addNewDestinationVariant(Destination* destination);

private slots:
    void onAddButtonClicked();
    void onColorChangeButtonClicked();
    void onCloseButtonClicked();

    void onWarehouseEditType();
    void onStartPointType();
    void onDestinationEditType();
    void onEndPointType();

private:
    QLineEdit*      name_line_edit_;
    QRadioButton*   choose_warehouse_type_;
    QRadioButton*   arbitrary_start_point_type_;
    QLabel*         start_name_label_;
    QLabel*         start_lat_label_;
    QLabel*         start_lng_label_;
    QLineEdit*      start_point_lat_;
    QLineEdit*      start_point_lng_;
    QRadioButton*   choose_destination_type_;
    QRadioButton*   arbitrary_end_point_type_;
    QLabel*         stop_name_label_;
    QLabel*         stop_lat_label_;
    QLabel*         stop_lng_label_;
    QLineEdit*      end_point_lat_;
    QLineEdit*      end_point_lng_;
    QPushButton*    add_route_button_;
    QPushButton*    color_change_button_;
    QColorDialog*   color_dialog_;
    QLabel*         warehouse_combo_label_;
    QComboBox*      warehouse_combo_;
    QLabel*         destination_combo_label_;
    QComboBox*      destination_combo_;
};

#endif // ORDERADDWIDGET_H
