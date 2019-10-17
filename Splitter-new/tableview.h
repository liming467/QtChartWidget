//tableview.h
#ifndef TABLEVIEW_H
#define TABLEVIEW_H
#include <QTableView>
#include <QStandardItemModel>


class TableView : public QTableView
{
public:
    explicit TableView(QWidget *parent = 0);

    void setTableHeader(int index,QString str);
    void setColumn(QVector<QPointF> &pointVector);

private:
    QStandardItemModel* model;

     void initTable();
};

#endif // TABLEVIEW_H
