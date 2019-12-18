#include "filter.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>

Filter::Filter()
{

}

int Filter::_Power_of_two(int n){
    int i = 0, m = 1;
    while (m < n) {
        m <<= 1;
        ++i;
    }
    return i;
}



void Filter::median_filter(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector){
    post_pointVector.clear();
    //you start coding your filter func here

    typedef double element;
    for (int i = m_radius/2; i < src_pointVector.size()-m_radius/2 ; ++i)
    {
        element *window=new element[m_radius];//window[n]
        for (int j = 0; j < m_radius; ++j){//for (int j = 0; j < n; ++j)
            window[j] = src_pointVector.at(i - m_radius/2 + j).y();
        }
        //sort the elements in window
        for (int j = 0; j < m_radius/2+1; ++j)//for (int j = 0; j < n/2+0.5; ++j)
        {            //  寻找当前窗口中最小元素的位置（前3个）
            int min = j;
            for (int k = j + 1; k < m_radius; ++k)//for (int k = j + 1; k < n; ++k)
                if (window[k] < window[min])
                    min = k;
            const element temp = window[j];
            window[j] = window[min];
            window[min] = temp;
        }
        post_pointVector.append(QPointF(src_pointVector.at(i - m_radius/2).x(),window[m_radius/2]));
        delete [] window;
    }

}

void Filter::average_filter(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector){
    post_pointVector.clear();

    typedef double element;//place the window and realize the move of window
    for (int i = m_radius/2; i < src_pointVector.size() - m_radius/2; ++i)
    {
        //get elements in window
        element *window=new element[m_radius];//window[n]
        for (int j = 0; j < m_radius; ++j){//for (int j = 0; j < n; ++j)
            window[j] = src_pointVector.at(i - m_radius/2 + j).y();
        }
        //sort window element
        double sum = 0;
        for (int j = 0; j <m_radius; ++j)//for (int j = 0; j < n/2+0.5; ++j)
        {//calculate the sum of current window
            sum+=window[j];
        }
        double average  = sum/m_radius;
        //Get result - the middle element
        post_pointVector.append(QPointF(src_pointVector.at(i - m_radius/2).x(),average ));
    }

}
void Filter::gauss_filter(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector){
    post_pointVector.clear();

    unsigned int size = m_radius;
    double sigma = m_sigma;
    double* gauss = new double[size];
    int real_radius = (size - 1) / 2;
    double MySigma = 2 *PI* sigma*sigma;
    double value = 0;
    for (int i = 0; i < size; i++){
        gauss[i] = exp(-(i - real_radius)*(i - real_radius) / MySigma);
        value += gauss[i];
    }
    for (int i = 0; i < size; i++){
        gauss[i] = gauss[i] / value;
    }

    typedef double element;//放置窗口并实现窗口的移动
    for (int i = m_radius/2; i < src_pointVector.size() - m_radius/2; ++i)
    {
        element *window=new element[m_radius];
        for (int j = 0; j < m_radius; ++j)
        {
            window[j] = src_pointVector.at(i - m_radius/2 + j).y();
        }

        double WeightedMean = 0;
         for (int  i = 0; i < m_radius; ++i)
        {
             WeightedMean += gauss[i] * window[i];
        }
        //   Get result - the weightmean element

        post_pointVector.append(QPointF(src_pointVector.at(i - m_radius/2).x(),WeightedMean));
    }

}


void Filter::fourier_transformation(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector){
    post_pointVector.clear();

    //you start coding your filter func here

    int count=src_pointVector.size();
    //double SamplingPeriod=(src_pointVector.at(count-1).x()-src_pointVector.at(0).x())/(count-1);
    double SamplingFreq=m_fequency;

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

}

/***************parameters explanation***************
*TD:input complex
*FD:output complex
*r: N is the power of 2(such as pow in 2^pow)
*/
void Filter::fft(std::complex<double> *TD, std::complex<double> *FD, int r){
    int count;         //傅里叶变换点数
    int    i, j, k;              //循环变量
    int    bfsize, p;         //中间变量
    double   angle;        //角度
    std::complex<double> *w, *x1, *x2, *x;
    //计算傅里叶变换点数
    count = 1 << r;

    //分配运算所需要的存储器
    w = new std::complex<double>[count / 2];
    x1 = new std::complex<double>[count];
    x2 = new std::complex<double>[count];

    //计算加权系数
    for (i = 0; i < count / 2; i++)
    {
        angle = -i*PI * 2 / count;
        w[i] = std::complex<double>(cos(angle), sin(angle));
    }
    //将时域点写入x1
    memcpy(x1, TD, sizeof(std::complex<double>)* count);

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

double Filter::median_filter_1d(double *input,const int windowSize){

    double *window=new double[windowSize];//window[n]
    for (int j = 0; j < windowSize; ++j){//for (int j = 0; j < n; ++j)
        window[j] = input[j];
    }
    //sort the elements in window
    for (int j = 0; j < windowSize/2+1; ++j)//for (int j = 0; j < n/2+0.5; ++j)
    {            //  寻找当前窗口中最小元素的位置（前3个）
        int min = j;
        for (int k = j + 1; k < windowSize; ++k)//for (int k = j + 1; k < n; ++k)
            if (window[k] < window[min])
                min = k;
        const double temp = window[j];
        window[j] = window[min];
        window[min] = temp;
    }
    double result=window[windowSize/2];
    delete [] window;

    return result;

}

double Filter::average_filter_1d(double *input,const int windowSize){
    double result=0.0;
    for (int i=0;i<windowSize;++i) {
        result+=input[i];
    }
    result/=windowSize;
    return result;
}

double Filter::gauss_filter_1d(double *input,const int windowSize){

    double sigma = m_sigma;
    double* gauss = new double[windowSize];
    int real_radius = (windowSize - 1) / 2;
    double MySigma = 2 *PI* sigma*sigma;
    double value = 0;
    for (int i = 0; i < windowSize; i++){
        gauss[i] = exp(-(i - real_radius)*(i - real_radius) / MySigma);
        value += gauss[i];
    }
    for (int i = 0; i < windowSize; i++){
        gauss[i] = gauss[i] / value;
    }

    double weightedMean = 0;
     for (int  i = 0; i < windowSize; ++i)
    {
         weightedMean += gauss[i] * input[i];
    }

    return weightedMean;

}
