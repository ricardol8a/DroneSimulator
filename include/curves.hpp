#ifndef _CURVES_HPP_
#define _CURVES_HPP_

#include "default.hpp"
#include <cmath>
#include "glm/glm.hpp"
using namespace glm;

inline const dvec3 line(const double &t)
{
	double x = 1.0 - 2.0 * t;
	double y = 0.0;
	double z = 0.0;
	return dvec3(x, y, z);
}

inline const dvec3 parabole(const double &t)
{
	double x = 1.0 - 2.0 * t;
	double y = 0.0;
	double z = 2.0 * pow(x, 2.0) - 1.0;
	return dvec3(x, y, z);
}

inline const dvec3 wave(const double &n, const double &t)
{
	double x = 1.0 - 2.0 * t;
	double y = 0.0;
	double z = sin(M_PI * n * t);
	return dvec3(x, y, z);
}

inline const dvec3 zigzag(const double &n, const double &t)
{
	double x = 1.0 - 2.0 * t;
	double y = 0.0;
	double z = 2.0 * abs(2.0 * ((n * t / 2.0 + 0.25) - floor(n * t / 2.0 + 0.75))) - 1.0;
	return dvec3(x, y, z);
}

inline const dvec3 spiral(const double &n, const double &t)
{
	double r = 1.0 - 1.0 * t;
	double x = r * cos(2.0 * M_PI * n * t);
	double y = r * sin(2.0 * M_PI * n * t);
	double z = 0.0;
	return dvec3(x, y, z);
}

inline const dvec3 circle(const double &t)
{
	double x = cos(2.0 * M_PI * t);
	double y = sin(2.0 * M_PI * t);
	double z = 0.0;
	return dvec3(x, y, z);
}

inline const dvec3 lemniscate(const double &t)
{
	double x = cos(2.0 * M_PI * t) / (1.0 + pow(sin(2.0 * M_PI * t), 2.0));
	double y = sin(2.0 * M_PI * t) * x;
	double z = 0.0;
	return dvec3(x, y, z);
}

inline const dvec3 peanut(const double &t)
{
	double r = sqrt(1.0 - 0.75 * pow(sin(2.0 * M_PI * t), 2.0));
	double x = r * cos(2.0 * M_PI * t);
	double y = r * sin(2.0 * M_PI * t);
	double z = 0.0;
	return dvec3(x, y, z);
}

inline const dvec3 heart(const double &t)
{
	double x = (13.0 * cos(2.0 * M_PI * t) + 5.0 * cos(4.0 * M_PI * t) - 2.0 * cos(6.0 * M_PI * t) + cos(8.0 * M_PI * t)) / 17.0;
	double y = 16.0 * pow(sin(2.0 * M_PI * t), 3.0) / 17.0;
	double z = 0.0;
	return dvec3(x, y, z);
}

inline const dvec3 fish(const double &t)
{
	double x = (19.0 * cos(2.0 * M_PI * t) - 19.0 * pow(sin(2.0 * M_PI * t), 2.0) / sqrt(2.0) + 1.0) / 20.0;
	double y = 19.0 * cos(2.0 * M_PI * t) * sin(2.0 * M_PI * t) / 20.0;
	double z = 0.0;
	return dvec3(x, y, z);
}

inline const dvec3 rose(const double &n, const double &t)
{
	double r = cos(M_PI * n * t);
	double x = r * cos((1.0 + !fmod(n, 2.0)) * M_PI * t);
	double y = r * sin((1.0 + !fmod(n, 2.0)) * M_PI * t);
	double z = 0.0;
	return dvec3(x, y, z);
}

inline const dvec3 hypocycloid(const double &n, const double &k, const double &t)
{
	double x = ((n - k) / n) * cos(k * 2.0 * M_PI * t) + (k / n) * cos((n - k) * 2.0 * M_PI * t);
	double y = ((n - k) / n) * sin(k * 2.0 * M_PI * t) - (k / n) * sin((n - k) * 2.0 * M_PI * t);
	double z = 0.0;
	return dvec3(x, y, z);
}

inline const dvec3 hypotrochoidtypea(const double &n, const double &k, const double &t)
{
	double x = ((n - k) / (n - 0.5 * k)) * cos(k * 2.0 * M_PI * t) + (k / (2.0 * n - k)) * cos((n - k) * 2.0 * M_PI * t);
	double y = ((n - k) / (n - 0.5 * k)) * sin(k * 2.0 * M_PI * t) - (k / (2.0 * n - k)) * sin((n - k) * 2.0 * M_PI * t);
	double z = 0.0;
	return dvec3(x, y, z);
}

inline const dvec3 hypotrochoidtypeb(const double &n, const double &k, const double &t)
{
	double x = ((n - k) / (n + k)) * cos(k * 2.0 * M_PI * t) + (2.0 * k / (n + k)) * cos((n - k) * 2.0 * M_PI * t);
	double y = ((n - k) / (n + k)) * sin(k * 2.0 * M_PI * t) - (2.0 * k / (n + k)) * sin((n - k) * 2.0 * M_PI * t);
	double z = 0.0;
	return dvec3(x, y, z);
}

inline const dvec3 epicycloid(const double &n, const double &k, const double &t)
{
	double x = ((n + k) / (n + 2.0 * k)) * cos(k * 2.0 * M_PI * t) + (k / (n + 2.0 * k)) * cos((n + k) * 2.0 * M_PI * t);
	double y = ((n + k) / (n + 2.0 * k)) * sin(k * 2.0 * M_PI * t) + (k / (n + 2.0 * k)) * sin((n + k) * 2.0 * M_PI * t);
	double z = 0.0;
	return dvec3(x, y, z);
}

inline const dvec3 epitrochoidtypea(const double &n, const double &k, const double &t)
{
	double x = ((n + k) / (n + 1.5 * k)) * cos(k * 2.0 * M_PI * t) + (k / (2.0 * n + 3.0 * k)) * cos((n + k) * 2.0 * M_PI * t);
	double y = ((n + k) / (n + 1.5 * k)) * sin(k * 2.0 * M_PI * t) + (k / (2.0 * n + 3.0 * k)) * sin((n + k) * 2.0 * M_PI * t);
	double z = 0.0;
	return dvec3(x, y, z);
}

inline const dvec3 epitrochoidtypeb(const double &n, const double &k, const double &t)
{
	double x = ((n + k) / (n + 3.0 * k)) * cos(k * 2.0 * M_PI * t) + (2.0 * k / (n + 3.0 * k)) * cos((n + k) * 2.0 * M_PI * t);
	double y = ((n + k) / (n + 3.0 * k)) * sin(k * 2.0 * M_PI * t) + (2.0 * k / (n + 3.0 * k)) * sin((n + k) * 2.0 * M_PI * t);
	double z = 0.0;
	return dvec3(x, y, z);
}

inline const dvec3 star(const double &t)
{
	return hypocycloid(5.0, 2.0, t);
}

inline const dvec3 limacon(const double &t)
{
	return epitrochoidtypeb(1.0, 1.0, t);
}

inline const dvec3 polygon(const double &n, const double &t)
{
	double x = cos(M_PI / n) * cos((2.0 * floor(n * t) + 1.0) * M_PI / n) - (2.0 * n * t - 2.0 * floor(n * t) - 1.0) * sin(M_PI / n) * sin((2.0 * floor(n * t) + 1.0) * M_PI / n);
	double y = cos(M_PI / n) * sin((2.0 * floor(n * t) + 1.0) * M_PI / n) + (2.0 * n * t - 2.0 * floor(n * t) - 1.0) * sin(M_PI / n) * cos((2.0 * floor(n * t) + 1.0) * M_PI / n);
	double z = 0.0;
	return dvec3(x, y, z);
}

inline const dvec3 hypoellipse(const double &t)
{
	double x = pow(abs(cos(2.0 * M_PI * t)), 8.0) * ((cos(2.0 * M_PI * t) > 0) - (cos(2.0 * M_PI * t) < 0));
	double y = pow(abs(sin(2.0 * M_PI * t)), 8.0) * ((sin(2.0 * M_PI * t) > 0) - (sin(2.0 * M_PI * t) < 0));
	double z = 0.0;
	return dvec3(x, y, z);
}

inline const dvec3 hyperellipse(const double &t)
{
	double x = sqrt(abs(cos(2.0 * M_PI * t))) * ((cos(2.0 * M_PI * t) > 0) - (cos(2.0 * M_PI * t) < 0));
	double y = sqrt(abs(sin(2.0 * M_PI * t))) * ((sin(2.0 * M_PI * t) > 0) - (sin(2.0 * M_PI * t) < 0));
	double z = 0.0;
	return dvec3(x, y, z);
}

inline const dvec3 helix(const double &n, const double &t)
{
	double z = 1.0 - 2.0 * t;
	double x = cos(2.0 * M_PI * n * t);
	double y = sin(2.0 * M_PI * n * t);
	return dvec3(x, y, z);
}

inline const dvec3 swirl(const double &n, const double &t)
{
	double r = 1.0 - 1.0 * t;
	double x = r * cos(2.0 * M_PI * n * t);
	double y = r * sin(2.0 * M_PI * n * t);
	double z = 1.0 - 2.0 * t;
	return dvec3(x, y, z);
}

inline const dvec3 ring(const double &n, const double &t)
{
	double x = cos(2.0 * (1.0 + fmod(n, 2.0)) * M_PI * t);
	double y = sin(2.0 * (1.0 + fmod(n, 2.0)) * M_PI * t);
	double z = sin((1.0 + fmod(n, 2.0)) * M_PI * n * t);
	return dvec3(x, y, z);
}

inline const dvec3 crown(const double &n, const double &t)
{
	double x = cos(M_PI / n) * cos((2.0 * floor((1.0 + fmod(n, 2.0)) * n * t) + 1.0) * M_PI / n) - ((1.0 + fmod(n, 2.0)) * 2.0 * n * t - 2.0 * floor((1.0 + fmod(n, 2.0)) * n * t) - 1.0) * sin(M_PI / n) * sin((2.0 * floor((1.0 + fmod(n, 2.0)) * n * t) + 1.0) * M_PI / n);
	double y = cos(M_PI / n) * sin((2.0 * floor((1.0 + fmod(n, 2.0)) * n * t) + 1.0) * M_PI / n) + ((1.0 + fmod(n, 2.0)) * 2.0 * n * t - 2.0 * floor((1.0 + fmod(n, 2.0)) * n * t) - 1.0) * sin(M_PI / n) * cos((2.0 * floor((1.0 + fmod(n, 2.0)) * n * t) + 1.0) * M_PI / n);
	double z = 2.0 * abs(2.0 * ((1.0 + fmod(n, 2.0)) * n * t / 2.0 - floor((1.0 + fmod(n, 2.0)) * n * t / 2.0 + 0.5))) - 1.0;
	return dvec3(x, y, z);
}

inline const dvec3 torus(const double &n, const double &t)
{
	double r = 0.75 + 0.25 * cos((1.0 + fmod(n, 2.0)) * M_PI * n * t);
	double x = r * cos(2.0 * (1.0 + fmod(n, 2.0)) * M_PI * t);
	double y = r * sin(2.0 * (1.0 + fmod(n, 2.0)) * M_PI * t);
	double z = 0.25 * sin((1.0 + fmod(n, 2.0)) * M_PI * n * t);
	return dvec3(x, y, z);
}

inline const dvec3 lissajous(const double &t)
{
	double x = cos(6.0 * M_PI * t);
	double y = sin(4.0 * M_PI * t);
	double z = sin(14.0 * M_PI * t);
	return dvec3(x, y, z);
}

#endif
