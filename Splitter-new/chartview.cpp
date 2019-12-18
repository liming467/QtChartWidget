#include "chartview.h"

ChartView::ChartView(QWidget *parent) :
    QChartView(parent),
    m_chart(new QChart)
{    
    initChart();
}


void ChartView::initChart(){

    series = new QLineSeries();
    m_chart->addSeries(series);

    m_axisX=new QValueAxis();
    m_axisX->setTitleFont(QFont("Microsoft YaHei", 10, QFont::Normal));
    m_axisX->setGridLineVisible(true);
    m_axisX->setTickCount(10);//set the major tick count,have 10-1 space
    m_chart->addAxis(m_axisX,Qt::AlignBottom);


    m_axisY=new QValueAxis();
    m_axisY->setTitleFont(QFont("Microsoft YaHei", 10, QFont::Normal));
    m_axisY->setGridLineVisible(true);
    m_axisY->setTickCount(10);
    m_chart->addAxis(m_axisY,Qt::AlignLeft);


    series->attachAxis(m_axisX);
    series->attachAxis(m_axisY);
    //series->setColor(QColor(0, 0, 0));//set the color of curve

    m_chart->legend()->hide();

    setChart(m_chart);
    setRenderHint(QPainter::Antialiasing);//抗锯齿渲染

}


void ChartView::setAxisLabel(QString labelX,QString labelY){

    m_axisX->setTitleText(labelX);
    m_axisY->setTitleText(labelY);
}

void ChartView::setTitle(QString str){
    m_chart->setTitle(str);
}

void ChartView::addChartData(QVector<QPointF> &data){

    if(!data.size())
        return;

    qreal minX,maxX,minY,maxY;

    minX=maxX=data.at(0).x();
    minY=maxY=data.at(0).y();

    for (int i=0;i<data.size();i++) {
        minX = minX<data.at(i).x()?minX:data.at(i).x();
        maxX = maxX>data.at(i).x()?maxX:data.at(i).x();
        minY = minY<data.at(i).y()?minY:data.at(i).y();
        maxY = maxY>data.at(i).y()?maxY:data.at(i).y();

    }

    series->replace(data);


    m_axisX->setRange(minX, maxX);
    m_axisY->setRange(minY - (maxY - minY)*0.05, maxY + (maxY - minY)*0.05);

}
