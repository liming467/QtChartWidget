#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <tableview.h>
#include <chartview.h>
#include <QTabWidget>
#include <QSplitter>
#include <complex>
#include <vector>


namespace Ui {
class MainWindow;
}

QT_CHARTS_USE_NAMESPACE
using namespace std;



class MainWindow : public QMainWindow
{
    Q_OBJECT



public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void setRadius(int rad,double sig);
    //Dialog dlg;
    void medianClicked();
    void averageClicked();
    void gaussClicked();
    void fourierTransformationClicked();
    void fftClicked();
    void exampleClicked();

signals:
    void setSpinboxEnable(bool);

private:


    Ui::MainWindow *ui;

    int radius=3;
    double m_sigma=3.0;




    QTabWidget *tabWidget;
    TableView* tableView1;
    QSplitter * splitter;
    TableView* tableView2;
    ChartView *m_chartview;    
    QVector<QPointF> source_pointVector;
    QVector<QPointF> post_pointVector;

    void _Read_data();
    void initTable();
    void initChart();
    void initSplitter();
    void initTabWidget();


    //Filter function plugin example
    void Filter(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector);


    void medianFilter(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector);
    void averageFilter(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector);
    void gaussFilter(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector);
    void FourierTransformation(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector);
    //void zhangFilter(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector);    
    void fft_master(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector);
    void example(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector);
    int powerOfTwo(int n);
    void FFT(complex<double> *TD, complex<double> *FD, int r);



};

#endif // MAINWINDOW_H
