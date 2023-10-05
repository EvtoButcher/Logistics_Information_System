#ifndef ROUTEWIDGET_H
#define ROUTEWIDGET_H

#include <QWidget>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#include "RouteModel.h"

class RouteWidget : public QWidget
{
    Q_OBJECT

    QLineEdit*      name_line_edit_;
    QLineEdit*      start_point_lat_;
    QLineEdit*      start_point_lng_;
    QLineEdit*      end_point_lat_;
    QLineEdit*      end_point_lng_;
    QPushButton*    add_route_button_;

public:
    explicit RouteWidget(QWidget *parent = nullptr);
    ~RouteWidget() = default;

    RouteModel& GetRouteModel();

private:
    RouteModel route_model;


//signals:
//    void routeAdded();

public slots:
    void onAddRouteButtonClicked();
};

#endif // ROUTEWIDGET_H
