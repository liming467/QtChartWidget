#ifndef FILTER_H
#define FILTER_H
#include <QVector>
#include <complex>

const double PI=3.14159265358979323846;
class Filter
{
public:
    Filter();

    void median_filter(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector);
    void average_filter(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector);
    void gauss_filter(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector);
    void fourier_transformation(QVector<QPointF> &src_pointVector,QVector<QPointF> &post_pointVector);//forward
    void fft(std::complex<double> *TD, std::complex<double> *FD, int r);

    //1d realtime filter
    double median_filter_1d(double *input,const int windowSize);
    double average_filter_1d(double *input,const int windowSize);
    double gauss_filter_1d(double *input,const int windowSize);


private:
    int m_points_number;
    unsigned int m_ength=0;
    double m_fequency=1.0;
    unsigned int m_radius=3;//a parameter for median,average,gauss filter
    double m_sigma=3.0;//as a gauss filter parameter

    int _Power_of_two(int n);

};

#endif // FILTER_H
