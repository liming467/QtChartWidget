#include "tableview.h"
#include <QHeaderView>

TableView::TableView(QWidget *parent):
    QTableView(parent)
{
    initTable();

}

void TableView::initTable(){

   model = new QStandardItemModel();
   setModel(model);

   setShowGrid(true);
   horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
   setEditTriggers(QAbstractItemView::NoEditTriggers);
   //verticalHeader()->hide();
}


void TableView::setTableHeader(int index,QString str){
    model->setHorizontalHeaderItem(index, new QStandardItem(str));
}


void TableView::setColumn(QVector<QPointF> &pointVector){

    for(int i=0;i<pointVector.size();i++){
        model->setItem(i, 0, new QStandardItem(QString::number(pointVector.at(i).x())));
        model->setItem(i, 1, new QStandardItem(QString::number(pointVector.at(i).y())));
        model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        model->item(i, 1)->setTextAlignment(Qt::AlignCenter);
    }
}

inline void TableView::setItem(int row_index,int col_index,double item){
    model->setItem(row_index, col_index, new QStandardItem(QString::number(item)));
    model->item(row_index, col_index)->setTextAlignment(Qt::AlignCenter);
}







