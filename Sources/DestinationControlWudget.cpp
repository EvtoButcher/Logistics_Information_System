#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QStringList>

#include "Headers/DestinationControlWudget.h"
#include "Headers/CreateDestinationDialog.h"

DestinationWudget::DestinationWudget(QWidget *parent)
    : QWidget(parent)
{
    auto bold_font = QFont("Ubuntu", 11, QFont::Bold);

    destination_label_ = new QLabel("Create Destination", this);
    destination_label_->setFont(bold_font);
    destination_table_label_ = new QLabel("Destinations", this);

    destination_table_ = new QTableWidget(0, 2, this);
    QStringList destination_table_colum;
    destination_table_colum << "code" << "position";
    destination_table_->setHorizontalHeaderLabels(destination_table_colum);

    auto button_lay = new QHBoxLayout();

    add_destination_button_ = new QPushButton("add a new destination", this);
    delete_destination_button_ = new QPushButton("delete a warehouse", this);
    delete_destination_button_->setStyleSheet("QPushButton {color: rgba(183, 22, 26, 80);}");
    delete_destination_button_->setEnabled(false);

    button_lay->addWidget(add_destination_button_);
    button_lay->addSpacing(10);
    button_lay->addWidget(delete_destination_button_);

    create_destination_dialog_ = new CreateDestinationDialog(this);

    setLayout(new QVBoxLayout(this));

    layout()->addWidget(destination_label_);
    layout()->setSpacing(10);
    layout()->addWidget(destination_table_label_);
    layout()->addWidget(destination_table_);
    layout()->addItem(button_lay);

    connect(add_destination_button_, &QAbstractButton::clicked, [=](){create_destination_dialog_->exec();});
}
