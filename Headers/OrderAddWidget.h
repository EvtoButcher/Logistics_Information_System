#ifndef ORDERADDWIDGET_H
#define ORDERADDWIDGET_H

#include <QWidget>

#include "RouteModel.h"

class QLineEdit;
class QDockWidget;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QPushButton;
class QColorDialog;

class OrderAddWidget: public QWidget
{
    Q_OBJECT

public:
    explicit OrderAddWidget(QWidget *parent = nullptr);
    ~OrderAddWidget() = default;

signals:
     void addRoute(const RouteInfo&);

private slots:
    void onAddButtonClicked();
    void onColorChangeButtonClicked();
    void onCloseButtonClicked();

private:
    QLineEdit*      name_line_edit_;
    QLineEdit*      start_point_lat_;
    QLineEdit*      start_point_lng_;
    QLineEdit*      end_point_lat_;
    QLineEdit*      end_point_lng_;
    QPushButton*    add_route_button_;
    QPushButton*    color_change_button_;
    QColorDialog*   color_dialog_;
};

#endif // ORDERADDWIDGET_H
