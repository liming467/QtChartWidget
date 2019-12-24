#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHeaderView>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QInputDialog>
#include "inputdialog.h"
#include <QMessageBox>

double M_PI=3.1415926535;

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
    connect(ui->actionFourier_Transformation,SIGNAL(triggered()),this,SLOT(fourierTransformationClicked()));
    connect(ui->actionFFT,SIGNAL(triggered()),this,SLOT(fftClicked()));
    connect(ui->actionExample,SIGNAL(triggered()),this,SLOT(exampleClicked()));

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
    tabWidget->addTab(splitter, tr("Configuration"));
}



void MainWindow::_Read_data()
{
    QString name=QFileDialog::getOpenFileName(this,tr("open File"),".",tr("Text Files(*.txt)"));
    //QString name="D:/newData.csv";
    QMessageBox::information(this,"File Name:",name);



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
    if(src_pointVector.isEmpty()){
        QMessageBox::information(this,"Warning","No source file to process!");
        return;
    }


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
    if(src_pointVector.isEmpty()){
        QMessageBox::information(this,"Warning","No source file to process!");
        return;
    }


    /*InputDialog dlg;
    connect(&dlg,SIGNAL(emitRadius(int,int)),this,SLOT(setRadius(int,int)));
    int nRet=dlg.exec();
    if(nRet == QDialog::Rejected)
        return;*/

    post_pointVector.clear();
    //you start coding your filter func here

    radius=3;


    double average=0.0;
    typedef double element;//放置窗口并实现窗口的移动
    element *window=new element[radius];//window[n]
    for (int i = radius/2; i < src_pointVector.size()-radius/2 ; ++i)
    {
        //   取出窗口的元素

        int count=0;
        for (int j = i - radius/2; j <= i+radius/2; ++j){//for (int j = 0; j < n; ++j)
            window[count] = src_pointVector.at(j).y();
            count++;
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

        average=window[radius/2];


        post_pointVector.append(QPointF(i - radius/2,average));

    }

    delete [] window;

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
    if(src_pointVector.isEmpty()){
        QMessageBox::information(this,"Warning","No source file to process!");
        return;
    }


    InputDialog dlg;
    connect(&dlg,SIGNAL(emitRadius(int,int)),this,SLOT(setRadius(int,int)));
    int nRet=dlg.exec();
    if(nRet == QDialog::Rejected)
        return;

    post_pointVector.clear();

    //you start coding your filter func here

    for (int i = radius/2; i < src_pointVector.size()-radius/2; ++i)
    {
        double sum = 0;
        for (int j = i-radius/2; j <= i+radius/2; ++j){
            sum+=src_pointVector.at(j).y();
        }

        sum/=radius;
        post_pointVector.append(QPointF(i - radius/2,sum ));
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
    if(src_pointVector.isEmpty()){
        QMessageBox::information(this,"Warning","No source file to process!");
        return;
    }

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
    int real_radius = (size-1)/2;
    double MySigma = 2*sigma*sigma;
    double value = 0;

    for (int i = 0; i < size; i++)
    {
        gauss[i] = exp(-(i - real_radius)*(i - real_radius) / MySigma);
        value += gauss[i];
    }

    for (int i = 0; i < size; i++)
    {
        gauss[i] /= value;
    }

    typedef double element;//放置窗口并实现窗口的移动
    element *window=new element[size];
    double WeightedMean = 0.0;
    int count=0;
    for (int i = size/2; i < src_pointVector.size() - size/2; ++i)
    {
        count=0;
        WeightedMean = 0.0;
        for (int j = i-size/2; j <= i+size/2; ++j)
        {
            window[count] = src_pointVector.at(j).y();
            WeightedMean += gauss[count] * window[count];
            count++;
        }

        post_pointVector.append(QPointF(i - radius/2,WeightedMean));

        //post_pointVector.at(i - 2).y() = weightmean;
    }
    
    delete [] window;
    delete [] gauss;

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

void MainWindow::FourierTransformation(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector){
    if(src_pointVector.isEmpty()){
        QMessageBox::information(this,"Warning","No source file to process!");
        return;
    }

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
                Re1[i]+=src_pointVector.at(j).y()*cos(-2*M_PI*i*j/count);
                Im1[i]+=src_pointVector.at(j).y()*sin(-2*M_PI*i*j/count);
            }

    //第一个点和最后一个点(count=0和count/2)需要除以N,其余点的模需要除以N/2
    double x=0.0,y=0.0;
    y=sqrt(Re1[0]*Re1[0]+Im1[0]*Im1[0])/count;
    post_pointVector.append(QPointF(x,y));
    for (int i=1;i<count/2;i++){
        x=i*SamplingFreq/count;
        y=sqrt(Re1[i]*Re1[i]+Im1[i]*Im1[i])*2.0/count;
        post_pointVector.append(QPointF(x,y));
    }
    x=(count/2)*SamplingFreq/count;
    y=sqrt(Re1[count/2]*Re1[count/2]+Im1[count/2]*Im1[count/2])/count;
    post_pointVector.append(QPointF(x,y));


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


void MainWindow::fft_master(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector){
    if(src_pointVector.isEmpty()){
        QMessageBox::information(this,"Warning","No source file to process!");
        return;
    }


    post_pointVector.clear();

    //you start coding your filter func here

    int i=0;
    double x=0.0,y=0.0;
    const int count=src_pointVector.size();
    const int newCount=1 << powerOfTwo(count);
    double SamplingPeriod=(src_pointVector.at(count-1).x()-src_pointVector.at(0).x())/(count-1);
    double SamplingFreq=1.0/SamplingPeriod;

    complex<double>* in=new complex<double>[newCount];
    complex<double>* out=new complex<double>[newCount];

    for (i=0;i<count;i++) {
        x=src_pointVector.at(i).y();
        in[i]=complex<double>(x,0.0);

    }
    i=count;
    while (i<newCount) {
        in[i]=complex<double>(0.0,0.0);
        i++;
    }

    FFT(in, out, powerOfTwo(count));

    y=sqrt(out[0].real()*out[0].real()+out[0].imag()*out[0].imag())/newCount;
    post_pointVector.append(QPointF(0.0,y));
    for (int i=1;i<count/2;i++){
        x=i*SamplingFreq/newCount;
        y=sqrt(out[i].real()*out[i].real()+out[i].imag()*out[i].imag())*2.0/newCount;
        post_pointVector.append(QPointF(x,y));
    }
    x=(count/2)*SamplingFreq/newCount;
    y=sqrt(out[count/2].real()*out[count/2].real()+out[count/2].imag()*out[count/2].imag())/newCount;
    post_pointVector.append(QPointF(x,y));

    delete [] in;
    delete [] out;

    QString str = "";
    QFile file("D:/fft.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    for (i=0;i<count/2+1;i++) {
        str+=QString::number(post_pointVector.at(i).x());
        str+="\t";
        str+=QString::number(post_pointVector.at(i).y());
        str+="\n";
    }
    file.write(str.toUtf8());
    file.close();


    //set the label of x and y axies of source data
    tableView1->setTableHeader(0,"Time(s)");// x axis label of src data
    tableView1->setTableHeader(1,"Displacement1(mm)");//y axis label of src data

    //set the label of x and y axies of resulting data
    tableView2->setTableHeader(0,"Frequency");
    tableView2->setTableHeader(1,"Amplitude");

    tableView1->setColumn(src_pointVector);
    tableView2->setColumn(post_pointVector);
    m_chartview->addChartData(post_pointVector);

}

void MainWindow::example(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector){

    post_pointVector.clear();
    src_pointVector.clear();

    //you start coding your filter func here

    int i=0;
    double x=0.0,y=0.0;
    const int L=1000;
    int newCount=1 << powerOfTwo(L);
    double Fs=1000;
    double T=1.0/Fs;

    double time[L];
    double S[L];
    complex<double>* in=new complex<double>[newCount];
    complex<double>* out=new complex<double>[newCount];

    for (i=0;i<L;i++) {
        time[i]=i*T;
        //S[i]=0.7*sin(2*M_PI*50*time[i])+sin(2*M_PI*120*time[i]);
        S[i]=cos(2*M_PI*50*time[i]);
        in[i]=complex<double>(S[i],0.0);
        src_pointVector.append(QPointF(time[i],S[i]));
    }
    i=L;
    while (i<newCount) {
        in[i]=complex<double>(0.0,0.0);
        src_pointVector.append(QPointF(0.0,0.0));
        i++;
    }

    //FourierTransformation(src_pointVector,post_pointVector);

    FFT(in, out, powerOfTwo(L));

    y=sqrt(out[i].real()*out[i].real()+out[i].imag()*out[i].imag())/newCount;
    post_pointVector.append(QPointF(0.0,y));
    for (i=1;i<newCount/2;i++){
        x=i*Fs/newCount;
        y=sqrt(out[i].real()*out[i].real()+out[i].imag()*out[i].imag())*2.0/newCount;
        post_pointVector.append(QPointF(x,y));
    }
    x=(newCount/2)*Fs/newCount;
    y=sqrt(out[newCount/2].real()*out[newCount/2].real()+out[newCount/2].imag()*out[newCount/2].imag())/newCount;
    post_pointVector.append(QPointF(x,y));



    delete [] in;
    delete [] out;

    //set the label of x and y axies of source data
    tableView1->setTableHeader(0,"Time(s)");// x axis label of src data
    tableView1->setTableHeader(1,"Displacement");//y axis label of src data

    //set the label of x and y axies of resulting data
    tableView2->setTableHeader(0,"Freq");
    tableView2->setTableHeader(1,"Amp");

    tableView1->setColumn(src_pointVector);
    tableView2->setColumn(post_pointVector);
    m_chartview->addChartData(post_pointVector);
    m_chartview->setAxisLabel("Freq","Amp");

}

int MainWindow::powerOfTwo(int n) {
    int i = 0, m = 1;
    while (m < n) {
        m <<= 1;
        ++i;
    }
    return i;
}

void MainWindow::setRadius(int rad,double sig){
    radius=rad;
    m_sigma=sig;
}

void MainWindow::medianClicked(){
    medianFilter(source_pointVector,post_pointVector);
    m_chartview->setTitle("Median filter");
}


void MainWindow::averageClicked(){
    averageFilter(source_pointVector,post_pointVector);
    m_chartview->setTitle("Average filter");
}


void MainWindow::gaussClicked(){    
    gaussFilter(source_pointVector,post_pointVector);
    m_chartview->setTitle("Gauss filter");


}

void MainWindow::exampleClicked(){
    example(source_pointVector,post_pointVector);
    m_chartview->setTitle("Example");
}


//快速离散傅里叶变换
void MainWindow::fftClicked(){
    fft_master(source_pointVector,post_pointVector);
    m_chartview->setTitle("Fast fourier transformation: single-sided amplitude spectrum");
}


//离散傅里叶变换
void MainWindow::fourierTransformationClicked(){

    FourierTransformation(source_pointVector,post_pointVector);
    //fft1(source_pointVector,post_pointVector);
    m_chartview->setTitle("Naive fourier transformation:single-sided amplitude spectrum");
}

/***************parameters explanation***************
*TD:input complex
*FD:output complex
*r: N is the power of 2(such as pow in 2^pow)
*/
void  MainWindow::FFT(complex<double> *TD, complex<double> *FD, int r)
{

    int count;         //傅里叶变换点数
    int    i, j, k;              //循环变量
    int    bfsize, p;         //中间变量
    double   angle;        //角度
    complex<double> *w, *x1, *x2, *x;
    //计算傅里叶变换点数
    count = 1 << r;

    //分配运算所需要的存储器
    w = new complex<double>[count / 2];
    x1 = new complex<double>[count];
    x2 = new complex<double>[count];

    //计算加权系数
    for (i = 0; i < count / 2; i++)
    {
        angle = -i*M_PI * 2 / count;
        w[i] = complex<double>(cos(angle), sin(angle));
    }
    //将时域点写入x1
    memcpy(x1, TD, sizeof(complex<double>)* count);

    //采用蝶形算法惊醒快速傅里叶变换
    for (k = 0; k < r; k++)
    {
        for (j = 0; j < 1 << k; j++)
        {
            bfsize = 1 << (r - k);
            for (i = 0; i < bfsize / 2; i++)
            {
                p = j*bfsize;
                x2[i + p] = x1[i + p] + x1[i + p + bfsize / 2];
                x2[i + p + bfsize / 2] = (x1[i + p] - x1[i + p + bfsize / 2])*w[i*(1 << k)];
            }
        }
        x = x1;
        x1 = x2;
        x2 = x;
    }

    //重新排序
    for(j = 0; j < count; j++)
    {
        p = 0;
        for (i = 0; i < r; i++)
        {
            if (j&(1 << i))
            {
                p += 1 << (r - i - 1);
            }
        }
        FD[j] = x1[p];
    }
    //释放内存
    delete w;
    delete x1;
    delete x2;
}
