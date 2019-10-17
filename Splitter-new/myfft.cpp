
/*

 * myfft.cpp

 */

#include <stdio.h>

#include <stdlib.h>

#include <math.h>

#include "myfft.h"

#define PI 3.1415926

 /*------------按照定义进行一维复数离散傅里叶变换----------------------------------------

  * 原理：

  *     一维离散傅里叶变换公式为： X[k] = ∑x[n]*e^(-2πi * nk/N)，其中n=0,1,2,...,N-1

  *     一维离散傅里叶逆变换公式为： x[n] = 1/N * ∑X[k]*e^(2πi * nk/N)，其中k=0,1,2,...,N-1

  *     欧拉公式：e^ix = cos(x) + isin(x)

  *

  * 参数1：in            输入的一维复数数组

  * 参数2：n1            输入的一维复数数组的长度

  * 参数3：out           输出的一维复数数组

  * 参数4：n2            输出的一维复数数组的长度(必须和n1相等)

  * 参数5：line_width    数组下标的间隔(默认为1)，对于行变换等于1，对于列变换等于一行的宽度

  * 参数6：is_inv        是否是逆变换(默认不是)

  *

  * 过客 & 386520874@qq.com & 2016.01.25

  */

int myfft_by_define_c2c_1d(my_complex * in, int n1, my_complex * out, int n2, int line_width/*=1*/, int is_inv/*=0*/)

{
	if (n1 != n2) { return 0; }
	double _2PI_N = 2.0 * PI / n1;
	if (is_inv == 0) //正变换
	{
		for (int m = 0; m < n1; m++)
		{

			int M = m * line_width;

			for (int k = 0; k < n1; k++)

			{

				int K = k * line_width;

				out[M].r += in[K].r * cos(_2PI_N * m * k) + in[K].i * sin(_2PI_N * m * k); //计算实部

				out[M].i += -in[K].r * sin(_2PI_N * m * k) + in[K].i * cos(_2PI_N * m * k); //计算虚部

			}

		}

	}
	else if (is_inv == 1) //逆变换

	{

		for (int m = 0; m < n1; m++)

		{

			int M = m * line_width;

			for (int k = 0; k < n1; k++)

			{

				int K = k * line_width;

				out[M].r += in[K].r * cos(_2PI_N * m * k) - in[K].i * sin(_2PI_N * m * k); //计算实部

				out[M].i += in[K].r * sin(_2PI_N * m * k) + in[K].i * cos(_2PI_N * m * k); //计算虚部

			}

			out[M].r /= n1; //归一化

			out[M].i /= n1; //归一化

		}

	}
	return 1;
}

//按照定义进行二维复数离散傅里叶变换

int myfft_by_define_c2c_2d(my_complex * in, int m, int n, int is_inv/*=0*/)

{

	my_complex * temp = new my_complex[m * n];

	memset(temp, 0, sizeof(my_complex) * m * n);



	for (int y = 0; y < n; y++) //先行变换

	{

		myfft_by_define_c2c_1d(in + y * n, n, temp + y * n, n, 1, is_inv);

	}



	memset(in, 0, sizeof(my_complex) * m * n);

	for (int x = 0; x < m; x++) //再列变换

	{

		myfft_by_define_c2c_1d(temp + x, m, in + x, m, n, is_inv);

	}



	delete[] temp; temp = NULL;



	return 1;

}

/*------------进行一维复数离散快速傅里叶变换(基2)----------------------------------------

 * 原理：

 *     对于FFT算法，一般是基2型的，即要求被变换的数组长度必须是2的m幂次方，

 *     这种要求过于苛刻，当然相比于按照定义进行计算来说，速度是很快的。此外，

 *     还有基4的算法，以及混合基算法等，但都对数组长度N有严格的限制。

 *     所以想自己开发一个适合任意N的FFT算法，还是比较难的，目前还是调用FFTW库暂时凑合着用吧。

 *

 * 参数1：in            输入的一维复数数组

 * 参数2：n             输入/出的一维复数数组的长度

 * 参数3：line_width    数组下标的间隔(默认为1)，对于行变换等于1，对于列变换等于一行的宽度

 * 参数4：is_inv        是否是逆变换(默认不是)

 *

 * 过客 & 386520874@qq.com & 2016.01.25

 */

int myfft_c2c_1d_base2(my_complex * in, int n, int line_width/*=1*/, int is_inv/*=0*/)

{

	int ttt = n;

	int L = 1; //蝶形运算的层数

	while (ttt = ttt >> 1) { L++; }



	my_complex * X = in;



	int N = n;

	int len = 1 << L; //将输入的一维数组对齐为 N=2^L

	if (n * 2 == len) //刚好是2的幂

	{

		L -= 1;

	}
	else if (n < len) //说明n不是2的幂

	{

		//		printf("Erorr: n is not 2^m format.\n"); //非基2算法还有待开发

		//		return 0;



		N = len;

		X = new my_complex[N]; //扩充数组大小

		int aaa = sizeof(my_complex);

		memset(X, 0, sizeof(my_complex) * N); //全部初始化为0

		memcpy(X, in, sizeof(my_complex) * n); //复制数据

	}



	//--------数组倒位序 二进制(n0n1n2) => (n2n0n1)-------------------

	//例如： 3的二进制为 011，对应的倒位序为 110，即为6

	my_complex * XX = new my_complex[N];

	for (int i = 0; i < N; i++)

	{

		int p = 0;

		for (int f = 0; f < L; f++)

		{

			if (i & (1 << f))

			{

				p += 1 << (L - f - 1);

			}

		}

		XX[i] = X[p];

	}

	memcpy(X, XX, sizeof(my_complex) * N); //复制数据

	delete[] XX; XX = NULL;



	//---------旋转因子-------------------

	my_complex * W = new my_complex[N / 2];

	memset(W, 0, sizeof(my_complex) * N / 2); //全部初始化为0

	for (int i = 0; i < N / 2; i++)

	{

		W[i] = my_complex(cos(-2.0 * PI * i / N), sin(-2.0 * PI * i / N));

	}



	//---------FFT算法(基2)----------------

	for (int f = 0; f < L; f++) //蝶形运算层数

	{

		int G = 1 << (L - f);

		int num = 1 << f;



		for (int u = 0; u < num; u++) //每组的元素个数

		{

			for (int g = 0; g < G; g += 2) //每层的组数

			{

				my_complex X1 = X[g * num + u];

				my_complex X2 = X[(g + 1) * num + u];



				X[g * num + u] = X1 + W[u * (1 << (L - f - 1))] * X2; //蝶形运算

				X[(g + 1) * num + u] = X1 - W[u * (1 << (L - f - 1))] * X2; //蝶形运算

			}

		}

	}



	if (X != NULL && X != in) //说明另外申请了内存空间

	{

		memcpy(in, X, sizeof(my_complex) * n);

		if (X) { delete[] X; X = NULL; }

	}

	if (W != NULL) { delete[] W; W = NULL; }



	return 1;

}

int print_complex_array_1d(my_complex * in, int n)

{

	for (int i = 0; i < n; i++)

	{

		printf("%d:%f+i%f ", i, in[i].r, in[i].i);

	}

	printf("\n");



	return 1;

}

int print_complex_array_2d(my_complex * in, int m, int n)

{

	for (int y = 0; y < n; y++)

	{

		for (int x = 0; x < m; x++)

		{

			printf("%d_%d:%f+i%f ", y, x, (*(in + y * m + x)).r, (*(in + y * m + x)).i);

		}

		printf("\n");

	}

	printf("\n");



	return 1;

}
