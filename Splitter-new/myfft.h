#pragma once
/*

 * myfft.h

 */

#ifndef __MYFFT_H__

#define __MYFFT_H__

#include <windows.h>

typedef struct _my_complex
{
	double r; //复数实部

	double i; //复数虚部



	_my_complex() {}

	_my_complex(double _r, double _i) { r = _r; i = _i; }

	~_my_complex() {}



	_my_complex operator + (const _my_complex & c) //重载加号运算符

	{

		return _my_complex(r + c.r, i + c.i);

	}



	_my_complex operator - (const _my_complex & c) //重载减号运算符

	{

		return _my_complex(r - c.r, i - c.i);

	}



	_my_complex operator * (const _my_complex & c) //重载乘号运算符

	{

		return _my_complex(r * c.r - i * c.i, r * c.i + i * c.r);

	}

}my_complex;





int myfft_by_define_c2c_1d(my_complex * in, int n1, my_complex * out, int n2, int line_width = 1, int is_inv = 0); //按照定义进行一维复数离散傅里叶变换

int myfft_by_define_c2c_2d(my_complex * in, int m, int n, int is_inv = 0); //按照定义进行二维复数离散傅里叶变换



int myfft_c2c_1d_base2(my_complex * in, int n, int line_width = 1, int is_inv = 0); //进行一维复数离散快速傅里叶变换(基2)



int print_complex_array_1d(my_complex * in, int n);

int print_complex_array_2d(my_complex * in, int m, int n);





#endif //__MYFFT_H__

