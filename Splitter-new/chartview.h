//chartview.h
#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QValueAxis>
#include <QLineSeries>

QT_CHARTS_USE_NAMESPACE

class ChartView : public QChartView
{
public:
    ChartView(QWidget *parent = 0);

    void setAxisLabel(QString labelX,QString labelY);
    void addChartData(QVector<QPointF> &data);
    void setTitle(QString str);

private:
    QChart *m_chart;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    QLineSeries *series;


    void initChart();

};

#endif // CHARTVIEW_H
