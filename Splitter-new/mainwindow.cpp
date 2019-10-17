#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHeaderView>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QInputDialog>
#include "inputdialog.h"
#include "myfft.h"

const double PI=3.1415926;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),    
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tabWidget = new QTabWidget;
    initTable();
    initSplitter();
    initChart();
    initTabWidget();
    //connect(dlg,SIGNAL((int)),this,SLOT(setRadius(int)));
    connect(ui->actionMedian_Filter,SIGNAL(triggered()),this,SLOT(medianClicked()));
    connect(ui->actionAverage_Filter,SIGNAL(triggered()),this,SLOT(averageClicked()));
    connect(ui->actionGauss_Filter,SIGNAL(triggered()),this,SLOT(gaussClicked()));
    connect(ui->actionFourier_Transformation,SIGNAL(triggered()),this,SLOT(fftClicked()));

    setCentralWidget(tabWidget);
    _Read_data();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initChart(){

    m_chartview = new ChartView();

    m_chartview->setAxisLabel("Time(s)","Displacement(mm)");

}


void MainWindow::initTable(){

    tableView1 = new TableView(tabWidget);
    tableView1->setTableHeader(0,"Time(s)");
    tableView1->setTableHeader(1,"Displacement1(mm)");

    tableView2 = new TableView(tabWidget);
    tableView2->setTableHeader(0,"Time(s)");
    tableView2->setTableHeader(1,"Displacement2(mm)");

}

void MainWindow::initSplitter(){

    splitter = new QSplitter(tabWidget);
    splitter->addWidget(tableView1);
    splitter->addWidget(tableView2);

}


void MainWindow::initTabWidget(){

    tabWidget->addTab(m_chartview, tr("Post-processed curve"));
    tabWidget->addTab(splitter, tr("Source data and post-processed data"));
}



void MainWindow::_Read_data()
{
    QString name=QFileDialog::getOpenFileName(this,tr("open File"),".",tr("Text Files(*.txt)"));
    //QString name="‪3.txt";



   // QString string = QInputDialog::getText(this,tr("获取字符串"),tr("filter radius"),QLineEdit::Normal,tr("admin"),&ok);


    QFile readFile(name);
    int i=0;

    if(!readFile.open(QIODevice::ReadOnly | QFile::Truncate | QFile::Text))
    {
        QMessageBox::information(nullptr, "Warning", "Fail to read the file");
        return;
    }

    QTextStream readStream(&readFile);
    QString line;



    while(i<1000)
    {
        line=readStream.readLine();

        if(!line.isEmpty())
        {
            QStringList list=line.split(QRegExp(","),QString::SkipEmptyParts);
            source_pointVector.append(QPointF(list.at(0).toDouble(),list.at(1).toDouble()));
            //post_pointVector.append(QPointF(list.at(0).toDouble(),list.at(2).toDouble()));

            }

        i++;
    }

    readFile.close();


    tableView1->setColumn(source_pointVector);
    //tableView2->setColumn(post_pointVector);
    m_chartview->addChartData(source_pointVector);

    //QMessageBox::information(this,"Warning","Filter radius: "+QString::number(radius));

}

void MainWindow::Filter(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector){


    InputDialog dlg;
    connect(&dlg,SIGNAL(emitRadius(int,int)),this,SLOT(setRadius(int)));
    int nRet=dlg.exec();
    if(nRet == QDialog::Rejected)
        return;

    post_pointVector.clear();

    //you start coding your filter func here



    //set the label of x and y axies of source data
    tableView1->setTableHeader(0,"Time(s)");// x axis label of src data
    tableView1->setTableHeader(1,"Displacement1(mm)");//y axis label of src data

    //set the label of x and y axies of resulting data
    tableView2->setTableHeader(0,"Time(s)");
    tableView2->setTableHeader(1,"Displacement2(mm)");

    tableView1->setColumn(src_pointVector);
    tableView2->setColumn(post_pointVector);
    m_chartview->addChartData(post_pointVector);
}


void MainWindow::medianFilter(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector){


    InputDialog dlg;
    connect(&dlg,SIGNAL(emitRadius(int,int)),this,SLOT(setRadius(int,int)));
    int nRet=dlg.exec();
    if(nRet == QDialog::Rejected)
        return;

    post_pointVector.clear();
    //you start coding your filter func here

    typedef double element;//放置窗口并实现窗口的移动
    for (int i = radius/2; i < src_pointVector.size()-radius/2 ; ++i)
    {
        //   取出窗口的元素
        element *window=new element[radius];//window[n]
        for (int j = 0; j < radius; ++j){//for (int j = 0; j < n; ++j)
            window[j] = src_pointVector.at(i - radius/2 + j).y();
        }
        //   取出 窗口元素排序
        for (int j = 0; j < radius/2+1; ++j)//for (int j = 0; j < n/2+0.5; ++j)
        {            //  寻找当前窗口中最小元素的位置（前3个）
            int min = j;
            for (int k = j + 1; k < radius; ++k)//for (int k = j + 1; k < n; ++k)
                if (window[k] < window[min])
                    min = k;
            const element temp = window[j];
            window[j] = window[min];
            window[min] = temp;
        }

        //   Get result - the middle element
        //QMessageBox::information(this,"Warning",str1);

        post_pointVector.append(QPointF(src_pointVector.at(i - radius/2).x(),window[radius/2]));
        //post_pointVector.at(i - 2).y() = window[2];
        delete [] window;
    }

    //you finish coding your filter codes here

    //set the label of x and y axies of source data
    tableView1->setTableHeader(0,"Time(s)");// x axis label of src data
    tableView1->setTableHeader(1,"Displacement1(mm)");//y axis label of src data

    //set the label of x and y axies of resulting data
    tableView2->setTableHeader(0,"Time(s)");
    tableView2->setTableHeader(1,"Displacement2(mm)");

    tableView1->setColumn(src_pointVector);
    tableView2->setColumn(post_pointVector);
    m_chartview->addChartData(post_pointVector);
}


void MainWindow::averageFilter(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector)
{


    InputDialog dlg;
    connect(&dlg,SIGNAL(emitRadius(int,int)),this,SLOT(setRadius(int,int)));
    int nRet=dlg.exec();
    if(nRet == QDialog::Rejected)
        return;

    post_pointVector.clear();

    //you start coding your filter func here

    QString str1="";

    typedef double element;//放置窗口并实现窗口的移动
    for (int i = radius/2; i < src_pointVector.size() - radius/2; ++i)
    {
        //   取出窗口的元素
        element *window=new element[radius];//window[n]
        for (int j = 0; j < radius; ++j){//for (int j = 0; j < n; ++j)
            window[j] = src_pointVector.at(i - radius/2 + j).y();
        }
        //取出 窗口元素排序
        double sum = 0;
        for (int j = 0; j <radius; ++j)//for (int j = 0; j < n/2+0.5; ++j)
        {//寻找当前窗口中的和
            sum+=window[j];        }

        double average  = sum/radius;
        //   Get result - the middle element

        post_pointVector.append(QPointF(src_pointVector.at(i - radius/2).x(),average ));
        //post_pointVector.at(i - 2).y() = window[2];
    }


    //you finish coding your filter codes here

    //set the label of x and y axies of source data
    tableView1->setTableHeader(0,"Time(s)");// x axis label of src data
    tableView1->setTableHeader(1,"Displacement1(mm)");//y axis label of src data

    //set the label of x and y axies of resulting data
    tableView2->setTableHeader(0,"Time(s)");
    tableView2->setTableHeader(1,"Displacement2(mm)");

    tableView1->setColumn(src_pointVector);
    tableView2->setColumn(post_pointVector);
    m_chartview->addChartData(post_pointVector);
}


void MainWindow::gaussFilter(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector)
{

    InputDialog dlg;
    connect(&dlg,SIGNAL(emitRadius(int,double)),this,SLOT(setRadius(int,double)));
    connect(this,SIGNAL(setSpinboxEnable(bool)),&dlg,SLOT(setSpinEnable(bool)));
    emit setSpinboxEnable(true);

    int nRet=dlg.exec();
    if(nRet == QDialog::Rejected)
        return;

    post_pointVector.clear();

    //you start coding your filter func here

    int size = radius;
    double sigma = m_sigma;
    double* gauss = new double[size];
    int real_radius = (size - 1) / 2;
    double MySigma = 2 *PI* sigma*sigma;
    double value = 0;

    for (int i = 0; i < size; i++)
    {
        gauss[i] = exp(-(i - real_radius)*(i - real_radius) / MySigma);
        value += gauss[i];
    }

    for (int i = 0; i < size; i++)
    {
        gauss[i] = gauss[i] / value;
    }

    typedef double element;//放置窗口并实现窗口的移动
    for (int i = radius/2; i < src_pointVector.size() - radius/2; ++i)
    {
        element *window=new element[radius];
        for (int j = 0; j < radius; ++j)
        {
            window[j] = src_pointVector.at(i - radius/2 + j).y();
        }

        double WeightedMean = 0;
         for (int  i = 0; i < radius; ++i)
        {
             WeightedMean += gauss[i] * window[i];
        }
        //   Get result - the weightmean element

        post_pointVector.append(QPointF(src_pointVector.at(i - radius/2).x(),WeightedMean));

        //post_pointVector.at(i - 2).y() = weightmean;
    }

    QMessageBox::information(this,"Warnig","Sigma: "+QString::number(sigma));



    //you finish coding your filter codes here

    //set the label of x and y axies of source data
    tableView1->setTableHeader(0,"Time(s)");// x axis label of src data
    tableView1->setTableHeader(1,"Displacement1(mm)");//y axis label of src data

    //set the label of x and y axies of resulting data
    tableView2->setTableHeader(0,"Time(s)");
    tableView2->setTableHeader(1,"Displacement2(mm)");

    tableView1->setColumn(src_pointVector);
    tableView2->setColumn(post_pointVector);
    m_chartview->addChartData(post_pointVector);
}

void MainWindow::fftFilter(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector){

    //QMessageBox::information(this,"Warning","Enter fftFilter()!");
    post_pointVector.clear();

    //you start coding your filter func here

    int count=src_pointVector.size();
    double SamplingPeriod=(src_pointVector.at(count-1).x()-src_pointVector.at(0).x())/(count-1);
    double SamplingFreq=1.0/SamplingPeriod;

    double *Re1=nullptr;//实部
    Re1=new double[count/2+1];
    double *Im1=nullptr;//虚部
    Im1=new double[count/2+1];

    for(int i=0;i<count/2+1;i++)
            Re1[i]=Im1[i]=0.0;

    for(int i=0;i<count/2+1;i++)
            for(int j=0;j<count;j++)
            {
                Re1[i]+=src_pointVector.at(j).y()*cos(-2*PI*i*j/count);
                Im1[i]+=src_pointVector.at(j).y()*sin(-2*PI*i*j/count);
            }

    double x=0.0,y=0.0;
    y=sqrt(Re1[0]*Re1[0]+Im1[0]*Im1[0])/count;
    post_pointVector.append(QPointF(x,y));
    for (int i=1;i<count/2+1;i++){
        x=i*SamplingFreq/count;
        y=sqrt(Re1[i]*Re1[i]+Im1[i]*Im1[i])*2.0/count;
        post_pointVector.append(QPointF(x,y));
    }


    delete [] Re1;
    delete [] Im1;
    Re1=nullptr;
    Im1=nullptr;



    //set the label of x and y axies of source data
    tableView1->setTableHeader(0,"Time(s)");// x axis label of src data
    tableView1->setTableHeader(1,"Displacement1(mm)");//y axis label of src data

    //set the label of x and y axies of resulting data
    tableView2->setTableHeader(0,"Frequency(Hz)");
    tableView2->setTableHeader(1,"Amplitude");

    tableView1->setColumn(src_pointVector);
    tableView2->setColumn(post_pointVector);
    m_chartview->addChartData(post_pointVector);
}

void MainWindow::setRadius(int rad,double sig){
    radius=rad;
    m_sigma=sig;
}

void MainWindow::medianClicked(){
    medianFilter(source_pointVector,post_pointVector);

}


void MainWindow::averageClicked(){
    averageFilter(source_pointVector,post_pointVector);

}


void MainWindow::gaussClicked(){    
    gaussFilter(source_pointVector,post_pointVector);

}

void MainWindow::fftClicked(){
    fftFilter(source_pointVector,post_pointVector);   
}
