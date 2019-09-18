#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QDebug>
#include <QMessageBox>


MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget),
    chart(new QChart),
    timer(new QTimer),
    count(0)
{
    ui->setupUi(this);


    time=nullptr;
    point1=nullptr;
    point2=nullptr;

    pointsSize=200;

    initFontColor();
    initReadData();

    timer->setInterval(100);
    timer->start();


}

MainWidget::~MainWidget()
{
    delete ui;

    delete [] time;
    delete [] point1;
    delete [] point2;

    for (int i=0;i<pointsNum;i++) {
        delete [] tempDis[i];
    }
    delete [] tempDis;

}


void MainWidget::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0) {
        chart->zoom(1.1);
    } else {
        chart->zoom(10/1.1);
    }

    QWidget::wheelEvent(event);
}

void MainWidget::initUI()
{

    initChart();
}


void MainWidget::initChart()
{
    //chart->createDefaultAxes();

    /**修改**/
    axisX=new QValueAxis();
    axisX->setTitleFont(QFont("Microsoft YaHei", 10, QFont::Normal, true));
    axisX->setTitleText("Time(s)");
    axisX->setGridLineVisible(true);
    chart->addAxis(axisX,Qt::AlignBottom);

    axisY=new QValueAxis();
    axisY->setTitleFont(QFont("Microsoft YaHei", 10, QFont::Normal, true));
    axisY->setTitleText("Dis(mm)");
    axisY->setGridLineVisible(true);
    chart->addAxis(axisY,Qt::AlignLeft);
    for (int i=0;i<pointsNum;i++) {
        series[i]->attachAxis(axisX);
        series[i]->attachAxis(axisY);
    }
    /**修改**/





    chart->legend()->hide();

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);//抗锯齿渲染

    ui->mainHorLayout->addWidget(chartView);

}


void MainWidget::initSlot()
{
    connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    connect(ui->allRadioButton,SIGNAL(clicked()),this,SLOT(selectAll()));
    connect(ui->invertRadioButton,SIGNAL(clicked()),this,SLOT(invertSelect()));

    for (int i=0;i<pointsNum;i++) {
        connect(checkBoxVector.at(i),SIGNAL(toggled(bool)),this,SLOT(checkboxChanged()));
        connect(series[i], SIGNAL(hovered(QPointF, bool)), this, SLOT(tipSlot(QPointF,bool)));
    }

}



void MainWidget::addChartData(float time, float *pointsDis)
{

    float globalMinX=time;
    float globalMaxX=time;
    float globalMinY=pointsDis[0];
    float globalMaxY=pointsDis[0];


    for (int i=0;i<pointsNum;i++)
    {
        QVector<QPointF> oldData = series[i]->pointsVector();
        QVector<QPointF> data;

        if(oldData.size()<pointsSize){
            data=oldData;
        }
        else {
            oldData.removeFirst();
            data=oldData;
        }

        data.append(QPointF(time,pointsDis[i]));


        float minX=data.at(0).x();
        float maxX=data.at(0).x();
        float minY=data.at(0).y();
        float maxY=data.at(0).y();

        for (int j=0;j<data.size();j++) {
            if(minY>data.at(j).y())
                minY=data.at(j).y();
            if(maxY<data.at(j).y())
                maxY=data.at(j).y();
        }

        if(i==0)
        {
            for (int j=0;j<data.size();j++) {
                if(minX>data.at(j).x())
                    minX=data.at(j).x();
                if(maxX<data.at(j).x())
                    maxX=data.at(j).x();
            }
            globalMinX=minX;
            globalMaxX=maxX;
        }

        globalMinY=globalMinY<minY?globalMinY:minY;
        globalMaxY=globalMaxY>maxY?globalMaxY:maxY;



        series[i]->replace(data);


        tempDis[i][0]=pointsDis[i];
        tempDis[i][1]=maxY;
        tempDis[i][2]=minY;

    }

    addTableData(tempDis);

    axisX->setRange(globalMinX, globalMaxX);
    axisY->setRange(globalMinY-(globalMaxY-globalMinY)*0.1, globalMaxY+(globalMaxY-globalMinY)*0.1);

}



void MainWidget::setPointsNum(int num)
{

    if(num>9)
    {
        QMessageBox::information(this,"Warnning","The number of points exceed 9!");
        return;
    }

    tempDis=new float*[num];

    pointsNum=num;
    //model->setRowCount(num);



    for (int i=0;i<pointsNum;i++) {
        series[i]=new QLineSeries();
        series[i]->setColor(colorTable.at(i));
        chart->addSeries(series[i]);

        tempDis[i]=new float[3];
    }

    initUI();
    initTable(num);

    initSlot();
}




void  MainWidget::addTableData(float **pointsDis)
{
    for (int i=0;i<pointsNum;i++) {
        for (int j=0;j<3;j++) {
            model->setItem(i, j+1, new QStandardItem(QString::number(pointsDis[i][j])));
            model->item(i,j+1)->setForeground(QBrush(colorTable.at(i)));
            model->item(i, j+1)->setTextAlignment(Qt::AlignCenter);
        }
    }

}



void MainWidget::initTable(int pointsNumber)
{

    ui->allRadioButton->setChecked(true);

    //
    model = new QStandardItemModel();
    model->setColumnCount(4);
    model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("PointsNum")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("Displacements(mm)")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("Peak(mm)")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("Valley(mm)")));

    ui->tableView->setModel(model);

    //设置表格的单元为只读属性，即不能编辑
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //默认显示行头，如果你觉得不美观的话，我们可以将隐藏
    ui->tableView->verticalHeader()->hide();
    //ui->tableView->setColumnWidth(1,80);//第（0+1）列表格宽80
    ui->tableView->setColumnWidth(1,100);//第（0+1）列表格宽80

    //接上


    model->setRowCount(pointsNumber);

    QString str="";
    for (int i=0;i<pointsNumber;i++) {
        str="Point";
        str+=QString::number(i+1);
        QCheckBox *box=new QCheckBox(str,ui->tableView);
        box->setStyleSheet(strColor[i]);

        checkBoxVector.append(box);

        model->setItem(i, 0, new QStandardItem(""));
        ui->tableView->setIndexWidget(model->index(i,0),checkBoxVector.at(i));
        model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        checkBoxVector.at(i)->setChecked(true);

    }

}



void MainWidget::selectAll()
{
    for(int i=0;i<checkBoxVector.size();i++)
        checkBoxVector.at(i)->setChecked(true);
}



void MainWidget::initFontColor()
{

    colorTable.append(QColor(255,0,0));//red
    colorTable.append(QColor(0,0,255));//blue
    colorTable.append(QColor(0,255,0));//green
    colorTable.append(QColor(139,0,0));//dark red
    colorTable.append(QColor(255,255,0));//yellow
    colorTable.append(QColor(0,0,0));//black
    colorTable.append(QColor(128,42,42));//棕色
    colorTable.append(QColor(160,32,240));//purple
    colorTable.append(QColor(0,255,255));//青色

    strColor[0]="QCheckBox{color:rgb(255,0,0)}";
    strColor[1]="QCheckBox{color:rgb(0,0,255)}";
    strColor[2]="QCheckBox{color:rgb(0,255,0)}";
    strColor[3]="QCheckBox{color:rgb(139,0,0)}";
    strColor[4]="QCheckBox{color:rgb(255,255,0)}";
    strColor[5]="QCheckBox{color:rgb(128,42,42)}";
    strColor[7]="QCheckBox{color:rgb(160,32,240)}";
    strColor[8]="QCheckBox{color:rgb(0,255,255)}";

}


void  MainWidget::invertSelect()
{
    for(int i=0;i<checkBoxVector.size();i++){

        if(checkBoxVector.at(i)->checkState())
            checkBoxVector.at(i)->setChecked(false);
        else
            checkBoxVector.at(i)->setChecked(true);

    }

}




void  MainWidget::checkboxChanged()
{

    for (int i=0;i<pointsNum;i++) {
        if(checkBoxVector.at(i)->checkState()){
            series[i]->setVisible(true);
            //QMessageBox::information(this,"Warring",QString::number(i+1)+":true");
        }
        else {
            series[i]->setVisible(false);
            //QMessageBox::information(this,"Warring",QString::number(i+1)+":false");
        }
    }

}


//for example


void MainWidget::initReadData()
{

    //QMessageBox::information(nullptr, "Warning", "initReadData()");


    QString name="..\\3.txt";

    QFile readFile(name);
    int i=0;

    if(!readFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(nullptr, "Warning", "Fail to read the file");
        return;
    }

    QTextStream readStream(&readFile);
    QString line;

    //int j=0;
    //line=readStream.readLine();
    //while(!readStream.atEnd())
       // ++j;

    time=new float[10000];
    point1=new float[10000];
    point2= new float[10000];
    QString str="";



    while(i<10000)
    {
        //QMessageBox::information(nullptr, "Warning", "Enter into the do while loop!");

        line=readStream.readLine();

        if(!line.isEmpty())
        {
           // QMessageBox::information(nullptr, "Warning", "Enter into the if statement!");
            QStringList list=line.split(QRegExp(","),QString::SkipEmptyParts);

            time[i]=list.at(0).toFloat();
            point1[i]=list.at(1).toFloat();
            point2[i]=list.at(2).toFloat();

            if(i<20)
            {
                str+=QString::number(time[i]);
                str+=", ";
                str+=QString::number(point1[i]);
                str+=", ";
                str+=QString::number(point2[i]);
                str+="\n";
            }

            }

        i++;
    }

    readFile.close();

    //QMessageBox::information(this, "Warning", str);

}



void MainWidget::addData()
{

    float y[2];
    y[0]=point1[count];
    y[1]=point2[count];

    addChartData(time[count],y);


    count++;
}

void MainWidget::timerSlot()
{
    if (QObject::sender() == timer) {
        //updateData();

        addData();
    }
}






