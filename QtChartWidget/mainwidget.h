#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QChart>
#include <QLineSeries>
#include <QVector>
#include <QTimer>
#include <QStandardItemModel>
#include <QCheckBox>
#include <QValueAxis>

#include <QChartView>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();


    void setPointsNum(int num);
    void addChartData(float time, float *pointsDis);

private:
    void wheelEvent(QWheelEvent *event);

    void initUI();
    void initChart();
    void initSlot();
    void initTable(int pointsNumber);
    void initFontColor();


    void initReadData();
    void addData();



    Ui::MainWidget *ui;

    QChartView *chartView;
    QChart *chart;
    QLineSeries *series[8];
    int pointsSize;//用于画曲线的数据点数


    float *time;
    float *point1;
    float *point2;
    int pointsNum;

    float **tempDis;//store dis,mini dis, max dis for every points;

    QTimer *timer;

    quint16 count;




private slots:
    void timerSlot();

    void selectAll();
    void invertSelect();
    void checkboxChanged();



    //table

private:

    QVector<QCheckBox *> checkBoxVector;
    QString strColor[9];
    QVector<QColor> colorTable;
    QStandardItemModel *model;

    QValueAxis *axisX;
    QValueAxis *axisY;


    void addTableData(float **pointsDis);



};

#endif // MAINWIDGET_H
