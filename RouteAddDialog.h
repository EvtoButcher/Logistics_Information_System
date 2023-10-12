#ifndef ROUTEADDDIALOG_H
#define ROUTEADDDIALOG_H

#include <QDockWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QColorDialog>

#include "RouteModel.h"

class RouteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RouteDialog(QWidget *parent = nullptr);
    ~RouteDialog() = default;

signals:
     void addRouteToTable(const RouteInfo&);

private slots:
    void onAddButtonClicked();
    void onColorChangeButtonClicked();

private:
    QLineEdit*      name_line_edit_;
    QLineEdit*      start_point_lat_;
    QLineEdit*      start_point_lng_;
    QLineEdit*      end_point_lat_;
    QLineEdit*      end_point_lng_;
    QPushButton*    add_route_button_;
    QPushButton*    close_dialog_button_;
    QPushButton*    color_change_button_;
    QColorDialog*   color_dialog_;
    bool color_changed_ = false;
};

#endif // ROUTEADDDIALOG_H
