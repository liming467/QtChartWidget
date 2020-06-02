#pragma once
#include <QVector>
#include <complex>

const double PI = 3.14159265358979323846;
class Filter
{
public:
	Filter();

	void median_filter(QVector<QPointF> &src_pointVector, QVector<QPointF> &post_pointVector);
	void average_filter(QVector<QPointF> &src_pointVector, QVector<QPointF> &post_pointVector);
	void gauss_filter(QVector<QPointF> &src_pointVector, QVector<QPointF> &post_pointVector);
	void fourier_transformation(QVector<QPointF> &src_pointVector, QVector<QPointF> &post_pointVector);//forward
	void fft(std::complex<double> *TD, std::complex<double> *FD, int r);

	//1d realtime filter
	float median_filter_1d(QVector<float> input, const int windowSize);
	float average_filter_1d(QVector<float> input, const int windowSize);
	float gauss_filter_1d(double *input, const int windowSize);

	void analyst_median_filter(std::vector<float> &input, std::vector<float> &output, int windowSize);
	void analyst_average_filter(std::vector<float> &input, std::vector<float> &output, int windowSize);
	void analyst_fourier_transformation(std::vector<float> &input, QVector<QPointF> &output, float samplingFreq);
	void analyst_fft_1d(std::vector<float> &input, QVector<QPointF> &output, float samplingFreq);
	void analyst_psd(std::vector<float>& input, QVector<QPointF>& output, float samplingFreq);


private:
	int m_points_number;
	unsigned int m_ength = 0;
	double m_fequency = 1.0;
	unsigned int m_radius = 3;//a parameter for median,average,gauss filter
	double m_sigma = 3.0;//as a gauss filter parameter

	int _Power_of_two(int n);
	

};

