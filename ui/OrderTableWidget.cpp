#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QSqlRecord>
#include <QModelIndex>
#include <QLabel>
#include <QSqlTableModel>
#include <QTableView>
#include <QPushButton>
#include <QFuture>
#include <QtConcurrent>
#include <QColorDialog>

#include "OrderTableWidget.h"
#include "common.h"

OrderTable::OrderTable(const OrderDB* db, QWidget *parent)
    : QWidget{parent}
    , table_delegate(parent)
{
    auto table_lay = new QVBoxLayout(this);

    table_model_ = new QSqlTableModel(this, db->DB());
    table_model_->setTable(MAIN_TABLE);
    table_model_->setEditStrategy(QSqlTableModel::OnFieldChange);
    table_model_->select();

    table_view_ = new QTableView(this);
    table_view_->setItemDelegate(&table_delegate);
    table_view_->setModel(table_model_);
    table_view_->resizeColumnsToContents();
    table_view_->setColumnWidth(1, 1);
    table_view_->setColumnHidden(0,true);//id
    table_view_->setColumnHidden(2,true);//StartPos
    table_view_->setColumnHidden(3,true);//EndPos

    auto button_lay = new QHBoxLayout(this);
    button_lay->setSpacing(5);

    add_order_button_ = new QPushButton(tr("Create an order"), this);
    remove_route_button_ = new QPushButton(tr("delete an order"), this);
    remove_route_button_->setEnabled(false);

    button_lay->addWidget(add_order_button_);
    button_lay->addWidget(remove_route_button_);

    table_lay->addWidget(table_view_);
    table_lay->addItem(button_lay);

    connect(table_view_->selectionModel(), &QItemSelectionModel::selectionChanged, this, &OrderTable::rowSelected);
    connect(remove_route_button_, &QAbstractButton::clicked, this, &OrderTable::removeOrderButtonClicked);
    connect(add_order_button_, &QAbstractButton::clicked, this, &OrderTable::addOrderButtonClicked);
    connect(table_view_, &QAbstractItemView::doubleClicked, this, &OrderTable::onTableViewColorClicked);
}

void OrderTable::updateTable()
{
    table_model_->select();
    table_view_->resizeColumnsToContents();
}

void OrderTable::onTableViewColorClicked(const QModelIndex &index)
{
    table_view_->selectRow(index.row());

    if(index.column() != 1){
        return;
    }

    QItemSelectionModel* selectionModel = table_view_->selectionModel();
    QModelIndexList indexes = selectionModel->selectedRows();

    auto color_dialog = new QColorDialog(table_model_->data(indexes.at(0)).toInt() ,this);
    color_dialog->exec();

    auto color = color_dialog->currentColor().name();

    emit updateColor(indexes.at(0).data(Qt::DisplayRole).toInt(), index.row(), color);

    table_model_->selectRow(index.row());

    table_view_->clearSelection();
}

void OrderTable::orderAddWidgetIsVisible()
{
    add_order_button_->isVisible() ? add_order_button_->hide() : add_order_button_->show();
}

void OrderTable::addOrderButtonClicked()
{
    emit openOrderDialog();
}

void OrderTable::removeOrderButtonClicked()
{
    QItemSelectionModel* selectionModel = table_view_->selectionModel();
    QModelIndexList indexes = selectionModel->selectedRows();

    if(indexes.isEmpty()){
        return; //TODO: add error handling
    }

    auto route_index_to_delete = indexes.at(0).data(Qt::DisplayRole).toInt();

    emit removeRoute(route_index_to_delete, indexes.at(0).row());

    table_model_->select();
}

void OrderTable::rowSelected()
{
    QItemSelectionModel* selectionModel = table_view_->selectionModel();
    QModelIndexList indexes = selectionModel->selectedRows();

    if(!indexes.empty()) {
        remove_route_button_->setEnabled(true);
        for(int index = 0; index < indexes.size(); ++index){
            emit onSelectRouteOnTable(indexes[index].row());
        }
    }
    else {
        remove_route_button_->setEnabled(false);                       //
        for(int index = 0; index < table_model_->rowCount(); ++index){ // Not the best option
            emit onUnselectRouteOnTable();                             //
        }
    }
}

void OrderTable::routeOnMapSelected(int index)
{
    table_view_->selectRow(index);
}

void OrderTable::routeOnMapUnselected()
{
    table_view_->clearSelection();
}

void OrderTable::setPathCacheAndDistance()
{
    table_model_->select();
}



