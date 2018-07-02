#ifndef _CALCULUS_HPP_
#define _CALCULUS_HPP_

#include "default.hpp"
#include <cmath>
#include <vector>
#include <functional>
#include "glm/glm.hpp"
using namespace std;
using namespace glm;

template <typename type>
type Euler(function<type(double, type)> func, type y, double x, double h, double xmax)
{
	type k1;
	
	for (; x < xmax; x += h)
	{
		k1 = func(x, y);
		y += h * k1;
	}
	return y;
}

template <typename type>
vector<type> Euler(function<type(double, vector<type>)> func, vector<type> y, double x, double h, double xmax)
{
	vector<type> k1(y.size());
	
	for (; x < xmax; x += h)
	{
		for (unsigned int i = 0; i != y.size(); i++)
		{
			if (y.size() - i - 1)
				k1[i] = y[i + 1];
			else
				k1[i] = func(x, y);
		}
		for (unsigned int i = 0; i != y.size(); i++)
			y[i] += h * k1[i];
	}
	return y;
}

template <typename type>
type Midpoint(function<type(double, type)> func, type y, double x, double h, double xmax)
{
	type k1, k2;

	for (; x < xmax; x += h)
	{
		k1 = func(x, y);
		k2 = func(x + 0.5 * h, y + 0.5 * h * k1);
		y += h * k2;	
	}
	return y;
}

template <typename type>
vector<type> Midpoint(function<type(double, vector<type>)> func, vector<type> y, double x, double h, double xmax)
{
	vector<type> yn(y.size());
	vector<type> k1(y.size()), k2(y.size());

	for (; x < xmax; x += h)
	{
		for (unsigned int i = 0; i != y.size(); i++)
		{
			if (y.size() - i - 1)
				k1[i] = y[i + 1];
			else
				k1[i] = func(x, y);
		}
		for (unsigned int i = 0; i != y.size(); i++)
		{
			yn[i] = y[i] + 0.5 * h * k1[i];
			if (y.size() - i - 1)
				k2[i] = y[i + 1] + 0.5 * h * k1[i + 1];
			else
				k2[i] = func(x + 0.5 * h, yn);
		}
		for (unsigned int i = 0; i != y.size(); i++)
			y[i] += h * k2[i];
	}
	return y;
}

template <typename type>
type RungeKutta(function<type(double, type)> func, type y, double x, double h, double xmax)
{
	type k1, k2, k3, k4;

	for (; x < xmax; x += h)
	{
		k1 = func(x, y);
		k2 = func(x + 0.5 * h, y + 0.5 * h * k1);
		k3 = func(x + 0.5 * h, y + 0.5 * h * k2);
		k4 = func(x + h, y + h * k3);
		y += h * (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
	}
	return y;
}

template <typename type>
vector<type> RungeKutta(function<type(double, vector<type>)> func, vector<type> y, double x, double h, double xmax)
{
	vector<type> yn(y.size());
	vector<type> k1(y.size()), k2(y.size()), k3(y.size()), k4(y.size());

	for (; x < xmax; x += h)
	{
		for (unsigned int i = 0; i != y.size(); i++)
		{
			if (y.size() - i - 1)
				k1[i] = y[i + 1];
			else
				k1[i] = func(x, y);
		}
		for (unsigned int i = 0; i != y.size(); i++)
		{
			yn[i] = y[i] + 0.5 * h * k1[i];
			if (y.size() - i - 1)
				k2[i] = y[i + 1] + 0.5 * h * k1[i + 1];
			else
				k2[i] = func(x + 0.5 * h, yn);
		}
		for (unsigned int i = 0; i != y.size(); i++)
		{
			yn[i] = y[i] + 0.5 * h * k2[i];
			if (y.size() - i - 1)
				k3[i] = y[i + 1] + 0.5 * h * k2[i + 1];
			else
				k3[i] = func(x + 0.5 * h, yn);
		}
		for (unsigned int i = 0; i != y.size(); i++)
		{
			yn[i] = y[i] + h * k3[i];
			if (y.size() - i - 1)
				k4[i] = y[i + 1] + h * k3[i + 1];
			else
				k4[i] = func(x + 0.5 * h, yn);
		}
		for (unsigned int i = 0; i != y.size(); i++)
			y[i] += h * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]) / 6.0;
	}
	return y;
}

template <typename type>
type DormandPrince(function<type(double, type)> func, type y, double x, double h, double xmax, double tolerance )
{
	type yn;
	double err;
	type k1, k2, k3, k4, k5, k6, k7;

	while ( x < xmax )
	{
		h = fmin(h, xmax - x);
		err = 0.0;
		k1 = func(x, y);
		k2 = func(x + 0.2 * h, y + 0.2 * h * k1);
		k3 = func(x + 0.3 * h, y + h * ( 0.075 * k1 + 0.225 * k2) );
		k4 = func(x + 0.8 * h, y + h * ( 44.0 / 45.0 * k1 - 56.0 / 15.0 * k2 + 32.0 / 9.0 * k3) );
		k5 = func(x + 8.0 / 9.0 * h,  y + h * ( 19372.0 / 6561.0 * k1 - 25360.0 / 2187.0 * k2 + 64448.0 / 6561.0 * k3 - 212.0 / 729.0 * k4) );
		k6 = func(x + h, y + h * ( 9017.0 / 3168.0 * k1 - 355.0 / 33.0 * k2 + 46732.0 / 5247.0 * k3 + 49.0 / 176.0 * k4 - 5103.0 / 18656.0 * k5) );
		k7 = func(x + h, y + h * ( 35.0 / 384.0 * k1 + 500.0 / 1113.0 * k3 + 125.0 / 192.0 * k4 - 2187.0 / 6784.0 * k5 + 11.0 / 84.0 * k6) );
		yn = y + h * ( 5179.0 / 57600.0 * k1 + 7571.0 / 16695.0 * k3 + 393.0 / 640.0 * k4 - 92097.0 / 339200.0 * k5 + 187.0 / 2100.0 * k6 + 1.0 / 40.0 * k7);
		err = length(h * ( 71.0 / 57600.0 * k1 - 71.0 / 16695.0 * k3 + 71.0 / 1920.0 * k4 - 17253.0 / 339200.0 * k5 + 22.0 / 525.0 * k6 - 1.0 / 40.0 * k7));
		if (err < tolerance)
		{
			x += h;
			y = yn;
		}
		h *= fmin(fmax(pow(tolerance / (2.0 * err), 1.0 / 5.0), 0.25), 4.0);
	}
	return y;
}

template <typename type>
vector<type> DormandPrince(function<type(double, vector<type>)> func, vector<type> y, double x, double h, double xmax, double tolerance )
{
	vector<type> yn(y.size());
	double err, aux;
	vector<type> k1(y.size()), k2(y.size()), k3(y.size()), k4(y.size()), k5(y.size()), k6(y.size()), k7(y.size());

	while ( x < xmax )
	{
		h = fmin(h, xmax - x);
		err = 0.0;
		for (unsigned int i = 0; i != y.size(); i++)
		{
			if (y.size() - i - 1)
				k1[i] = y[i + 1];
			else
				k1[i] = func(x, y);
		}
		for (unsigned int i = 0; i != y.size(); i++)
		{
			yn[i] = y[i] + 0.2 * h * k1[i];
			if (y.size() - i - 1)
				k2[i] = y[i + 1] + 0.2 * h * k1[i + 1];
			else
				k2[i] = func(x + 0.2 * h, yn);
		}
		for (unsigned int i = 0; i != y.size(); i++)
		{
			yn[i] = y[i] + h * ( 0.075 * k1[i] + 0.225 * k2[i]);
			if (y.size() - i - 1)
				k3[i] = y[i + 1] + h * ( 0.075 * k1[i + 1] + 0.225 * k2[i + 1]);
			else
				k3[i] = func(x + 0.3 * h, yn);
		}
		for (unsigned int i = 0; i != y.size(); i++)
		{
			yn[i] = y[i] + h * ( 44.0 / 45.0 * k1[i] - 56.0 / 15.0 * k2[i] + 32.0 / 9.0 * k3[i]);
			if (y.size() - i - 1)
				k4[i] = y[i + 1] + h * ( 44.0 / 45.0 * k1[i + 1] - 56.0 / 15.0 * k2[i + 1] + 32.0 / 9.0 * k3[i + 1]);
			else
				k4[i] = func(x + 0.8 * h, yn);
		}
		for (unsigned int i = 0; i != y.size(); i++)
		{
			yn[i] = y[i] + h * ( 19372.0 / 6561.0 * k1[i] - 25360.0 / 2187.0 * k2[i] + 64448.0 / 6561.0 * k3[i] - 212.0 / 729.0 * k4[i]);
			if (y.size() - i - 1)
				k5[i] = y[i + 1] + h * ( 19372.0 / 6561.0 * k1[i + 1] - 25360.0 / 2187.0 * k2[i + 1] + 64448.0 / 6561.0 * k3[i + 1] - 212.0 / 729.0 * k4[i + 1]);
			else
				k5[i] = func(x + 8.0 / 9.0 * h, yn);
		}
		for (unsigned int i = 0; i != y.size(); i++)
		{
			yn[i] = y[i] + h * ( 9017.0 / 3168.0 * k1[i] - 355.0 / 33.0 * k2[i] + 46732.0 / 5247.0 * k3[i] + 49.0 / 176.0 * k4[i] - 5103.0 / 18656.0 * k5[i]);
			if (y.size() - i - 1)
				k6[i] = y[i + 1] + h * ( 9017.0 / 3168.0 * k1[i + 1] - 355.0 / 33.0 * k2[i + 1] + 46732.0 / 5247.0 * k3[i + 1] + 49.0 / 176.0 * k4[i + 1] - 5103.0 / 18656.0 * k5[i + 1]);
			else
				k6[i] = func(x + h, yn);
		}
		for (unsigned int i = 0; i != y.size(); i++)
		{
			yn[i] = y[i] + h * ( 35.0 / 384.0 * k1[i] + 500.0 / 1113.0 * k3[i] + 125.0 / 192.0 * k4[i] - 2187.0 / 6784.0 * k5[i] + 11.0 / 84.0 * k6[i]);
			if (y.size() - i - 1)
				k7[i] = y[i + 1] + h * ( 35.0 / 384.0 * k1[i + 1] + 500.0 / 1113.0 * k3[i + 1] + 125.0 / 192.0 * k4[i + 1] - 2187.0 / 6784.0 * k5[i + 1] + 11.0 / 84.0 * k6[i + 1]);
			else
				k7[i] = func(x + h, yn);
		}
		for (unsigned int i = 0; i != y.size(); i++)
		{
			yn[i] = y[i] + h * ( 5179.0 / 57600.0 * k1[i] + 7571.0 / 16695.0 * k3[i] + 393.0 / 640.0 * k4[i] - 92097.0 / 339200.0 * k5[i] + 187.0 / 2100.0 * k6[i] + 1.0 / 40.0 * k7[i]);
			aux = length(h * ( 71.0 / 57600.0 * k1[i] - 71.0 / 16695.0 * k3[i] + 71.0 / 1920.0 * k4[i] - 17253.0 / 339200.0 * k5[i] + 22.0 / 525.0 * k6[i] - 1.0 / 40.0 * k7[i]));
			if(aux > err)
				err = aux;
		}
		if (err < tolerance)
		{
			x += h;
			y = yn;
		}
		h *= fmin(fmax(pow(tolerance / (2.0 * err), 1.0 / 5.0), 0.25), 4.0);
	}
	return y;
}

template <typename type>
type Differentiation(function<type(double, type)> func, type y, double x, double h, double xmax)
{
	#if(MATH == 1)
	return Euler(func, y, x, h, xmax);
	#elif(MATH == 2)
	return Midpoint(func, y, x, h, xmax);
	#elif(MATH == 3)
	return RungeKutta(func, y, x, h, xmax);
	#elif(MATH == 4)
	return DormandPrince(func, y, x, h, xmax, 1.0e-6);
	#endif
}

template <typename type>
vector<type> Differentiation(function<type(double, vector<type>)> func, vector<type> y, double x, double h, double xmax)
{
	#if(MATH == 1)
	return Euler(func, y, x, h, xmax);
	#elif(MATH == 2)
	return Midpoint(func, y, x, h, xmax);
	#elif(MATH == 3)
	return RungeKutta(func, y, x, h, xmax);
	#elif(MATH == 4)
	return DormandPrince(func, y, x, h, xmax, 1.0e-6);
	#endif
}

#endif
