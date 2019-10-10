#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHeaderView>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>

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

    QMessageBox::information(this,"Warning","Your file path is : "+name);

    //name="‪E:/Qt/Splitter/3.txt";

    QFile readFile(name);
    int i=0;

    if(!readFile.open(QIODevice::ReadOnly | QIODevice::Text))
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


    //tableView1->setColumn(source_pointVector);
    //tableView2->setColumn(post_pointVector);
    //m_chartview->addChartData(source_pointVector);

    medianFilter(source_pointVector,post_pointVector);

}

void MainWindow::Filter(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector){

    post_pointVector.clear();

    //you start coding your filter func here



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


void MainWindow::medianFilter(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector){

    post_pointVector.clear();

    //you start coding your filter func here

    QString str1="";


    typedef double element;//放置窗口并实现窗口的移动
    for (int i = 2; i < src_pointVector.size() - 2; ++i)
    {
        str1="";
        //   取出窗口的元素
        element window[5];//window[n]
        for (int j = 0; j < 5; ++j){//for (int j = 0; j < n; ++j)
            window[j] = src_pointVector.at(i - 2 + j).y();
            str1+=QString::number(window[j]);
            str1+="  ";

        }
        str1+="\n";
        //   取出 窗口元素排序
        for (int j = 0; j < 3; ++j)//for (int j = 0; j < n/2+0.5; ++j)
        {            //  寻找当前窗口中最小元素的位置（前3个）
            int min = j;
            for (int k = j + 1; k < 5; ++k)//for (int k = j + 1; k < n; ++k)
                if (window[k] < window[min])
                    min = k;
            const element temp = window[j];
            window[j] = window[min];
            window[min] = temp;
        }
        //   Get result - the middle element

        for (int i=0;i<5;++i) {
            str1+=QString::number(window[i]);
            str1+="  ";
        }

        //QMessageBox::information(this,"Warning",str1);

        post_pointVector.append(QPointF(src_pointVector.at(i - 2).x(),window[2]));

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


void MainWindow::averageFilter(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector){

    post_pointVector.clear();

    //you start coding your filter func here

    QString str1="";

    typedef double element;//放置窗口并实现窗口的移动
    for (int i = 2; i < src_pointVector.size() - 2; ++i)
    {
        //   取出窗口的元素
        element window[5];//window[n]
        for (int j = 0; j < 5; ++j){//for (int j = 0; j < n; ++j)
            window[j] = src_pointVector.at(i - 2 + j).y();
        }
        //取出 窗口元素排序
        double sum = 0;
        for (int j = 0; j < 5; ++j)//for (int j = 0; j < n/2+0.5; ++j)
        {//寻找当前窗口中的和
            sum+=window[j];        }

        double average  = sum/5.00;
        //   Get result - the middle element

        post_pointVector.append(QPointF(src_pointVector.at(i - 2).x(),average ));
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

