#ifndef _GEOMETRY_HPP_
#define _GEOMETRY_HPP_

#include "default.hpp"
#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/component_wise.hpp"

using namespace glm;

inline const dvec3 car(dvec3 p)
{
	return p;
}
inline const dvec3 car(const double &x, const double &y, const double &z)
{
	return car(dvec3(x, y, z));
}
inline const dvec3 cyl(dvec3 p)
{
	if (p[0] < 0.0)
	{
		p[0] = -p[0];
		p[1] += M_PI;
	}
	p[1] = remainder(p[1], 2.0 * M_PI);
	return p;
}
inline const dvec3 cyl(const double &rho, const double &theta, const double &z)
{
	return cyl(dvec3(rho, theta, z));
}
inline const dvec3 sph(dvec3 p)
{
	if (p[0] < 0.0)
	{
		p[0] = -p[0];
		p[1] += M_PI;
	}
	p[1] = remainder(p[1], 2.0 * M_PI);
	if (p[1] < 0.0)
	{
		p[1] = -p[1];
		p[2] += M_PI;
	}
	p[2] = remainder(p[2], 2.0 * M_PI);
	return p;
}
inline const dvec3 sph(const double &r, const double &theta, const double &phi)
{
	return sph(dvec3(r, theta, phi));
}

inline const dvec3 cyl2car(const dvec3 &p)
{
	double x = p[0] * cos(p[1]);
	double y = p[0] * sin(p[1]);
	double z = p[2];
	return car(x, y, z);
}
inline const dvec3 sph2car(const dvec3 &p)
{
	double x = p[0] * sin(p[1]) * cos(p[2]);
	double y = p[0] * sin(p[1]) * sin(p[2]);
	double z = p[0] * cos(p[1]);
	return car(x, y, z);
}
inline const dvec3 car2cyl(const dvec3 &p)
{
	double rho = sqrt(p[0] * p[0] + p[1] * p[1]);
	double theta = 0.0; 
	if(rho)
		theta = atan2(p[1], p[0]);
	double z = p[2];
	return cyl(rho, theta, z);
}
inline const dvec3 sph2cyl(const dvec3 &p)
{
	double rho = p[0] * sin(p[1]);
	double theta = p[2];
	double z = p[0] * cos(p[1]);
	return cyl(rho, theta, z);
}
inline const dvec3 car2sph(const dvec3 &p)
{
	double r = sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
	double theta = 0.0;
	double phi = 0.0;
	if(r)
	{
		theta = atan2(sqrt(p[0] * p[0] + p[1] * p[1]), p[2]);
		if ((M_PI / 2.0) - abs(theta - (M_PI / 2.0)))
			phi = atan2(p[1], p[0]);
	}
	return sph(r, theta, phi);
}
inline const dvec3 cyl2sph(const dvec3 &p)
{
	double r = sqrt(p[0] * p[0] + p[2] * p[2]);
	double theta = 0.0;
	if(r)
		theta = atan2(p[0], p[2]);
	double phi = p[1];
	return sph(r, theta, phi);
}

inline const dvec3 cyl2car(const dvec3 &v, const dvec3 &p, const bool &b)
{
	if(b)
		return (v[0] * dvec3(p[0] / sqrt(p[0] * p[0] + p[1] * p[1]), p[1] / sqrt(p[0] * p[0] + p[1] * p[1]), 0.0) + v[1] * dvec3(-p[1] / sqrt(p[0] * p[0] + p[1] * p[1]), p[0] / sqrt(p[0] * p[0] + p[1] * p[1]), 0.0) + v[2] * dvec3(0.0, 0.0, 1.0));
	else
		return (v[0] * dvec3(cos(p[1]), sin(p[1]), 0.0) + v[1] * dvec3(-sin(p[1]), cos(p[1]), 0.0) + v[2] * dvec3(0.0, 0.0, 1.0));
}
inline const dvec3 sph2car(const dvec3 &v, const dvec3 &p, const bool &b)
{
	if(b)
		return (v[0] * dvec3(p[0] / sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]), p[1] / sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]), p[2] / sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2])) + v[1] * dvec3(p[0] * p[2] / sqrt((p[0] * p[0] + p[1] * p[1] + p[2] * p[2]) * (p[0] * p[0] + p[1] * p[1])), p[1] * p[2] / sqrt((p[0] * p[0] + p[1] * p[1] + p[2] * p[2]) * (p[0] * p[0] + p[1] * p[1])), -sqrt((p[0] * p[0] + p[1] * p[1]) / (p[0] * p[0] + p[1] * p[1] + p[2] * p[2]))) + v[2] * dvec3(-p[1] / sqrt(p[0] * p[0] + p[1] * p[1]), p[0] / sqrt(p[0] * p[0] + p[1] * p[1]), 0.0));
	else
		return (v[0] * dvec3(sin(p[1]) * cos(p[2]), sin(p[1]) * sin(p[2]), cos(p[1])) + v[1] * dvec3(cos(p[1]) * cos(p[2]), cos(p[1]) * sin(p[2]), -sin(p[1])) + v[2] * dvec3(-sin(p[2]), cos(p[2]), 0.0));
}
inline const dvec3 car2cyl(const dvec3 &v, const dvec3 &p, const bool &b)
{
	if(b)
		return (v[0] * dvec3(cos(p[1]), -sin(p[1]), 0.0) + v[1] * dvec3(sin(p[1]), cos(p[1]), 0.0) + v[2] * dvec3(0.0, 0.0, 1.0));
	else
		return (v[0] * dvec3(p[0] / sqrt(p[0] * p[0] + p[1] * p[1]), -p[1] / sqrt(p[0] * p[0] + p[1] * p[1]), 0.0) + v[1] * dvec3(p[1] / sqrt(p[0] * p[0] + p[1] * p[1]), p[0] / sqrt(p[0] * p[0] + p[1] * p[1]), 0.0) + v[2] * dvec3(0.0, 0.0, 1.0));
}
inline const dvec3 sph2cyl(const dvec3 &v, const dvec3 &p, const bool &b)
{
	if(b)
		return (v[0] * dvec3(p[0] / sqrt(p[0] * p[0] + p[2] * p[2]), 0.0, p[2] / sqrt(p[0] * p[0] + p[2] * p[2])) + v[1] * dvec3(p[2] / sqrt(p[0] * p[0] + p[2] * p[2]), 0.0, -p[0] / sqrt(p[0] * p[0] + p[2] * p[2])) + v[2] * dvec3(0.0, 1.0, 0.0));
	else
		return (v[0] * dvec3(sin(p[1]), 0.0, cos(p[1])) + v[1] * dvec3(cos(p[1]), 0.0, -sin(p[1])) + v[2] * dvec3(0.0, 1.0, 0.0));
}
inline const dvec3 car2sph(const dvec3 &v, const dvec3 &p, const bool &b)
{
	if(b)
		return (v[0] * dvec3(sin(p[1]) * cos(p[2]), cos(p[1]) * cos(p[2]), -sin(p[2])) + v[1] * dvec3(sin(p[1]) * sin(p[2]), cos(p[1]) * sin(p[2]), cos(p[2])) + v[2] * dvec3(cos(p[1]), -sin(p[1]), 0.0));
	else
		return (v[0] * dvec3(p[0] / sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]), p[0] * p[2] / sqrt((p[0] * p[0] + p[1] * p[1] + p[2] * p[2]) * (p[0] * p[0] + p[1] * p[1])), -p[1] / sqrt(p[0] * p[0] + p[1] * p[1])) + v[1] * dvec3(p[1] / sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]), p[1] * p[2] / sqrt((p[0] * p[0] + p[1] * p[1] + p[2] * p[2]) * (p[0] * p[0] + p[1] * p[1])), p[0] / sqrt(p[0] * p[0] + p[1] * p[1])) + v[2] * dvec3(p.z / sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]), -sqrt((p[0] * p[0] + p[1] * p[1]) / (p[0] * p[0] + p[1] * p[1] + p[2] * p[2])), 0.0));
}
inline const dvec3 cyl2sph(const dvec3 &v, const dvec3 &p, const bool &b)
{
	if(b)
		return (v[0] * dvec3(sin(p[1]), cos(p[1]), 0.0) + v[1] * dvec3(0.0, 0.0, 1.0) + v[2] * dvec3(cos(p[1]), -sin(p[1]), 0.0));
	else
		return (v[0] * dvec3(p[0] / sqrt(p[0] * p[0] + p[2] * p[2]), p[2] / sqrt(p[0] * p[0] + p[2] * p[2]), 0.0) + v[1] * dvec3(0.0, 0.0, 1.0) + v[2] * dvec3(p[2] / sqrt(p[0] * p[0] + p[2] * p[2]), -p[0] / sqrt(p[0] * p[0] + p[2] * p[2]), 0.0));
}

inline const dvec3 ang(const dvec3 &p)
{
	return dvec3(remainder(p[0], 2.0 * M_PI), remainder(p[1], 2.0 * M_PI), remainder(p[2], 2.0 * M_PI));
}

inline const dvec3 qua2eul(const dquat &q)
{
	double roll = atan2(2.0 * (q.w * q.x + q.y * q.z), 1.0 - 2.0 * (q.x * q.x + q.y * q.y));
	double pitch = asin(clamp(2.0 * (q.w * q.y - q.z * q.x), -1.0, 1.0));
	double yaw = atan2(2.0 * (q.w * q.z + q.x * q.y), 1.0 - 2.0 * (q.y * q.y + q.z * q.z));
	return dvec3(roll, pitch, yaw);
}

inline const dquat eul2qua(const dvec3 &p)
{
	double w = cos(p[0] / 2.0) * cos(p[1] / 2.0) * cos(p[2] / 2.0) + sin(p[0] / 2.0) * sin(p[1] / 2.0) * sin(p[2] / 2.0);
	double x = sin(p[0] / 2.0) * cos(p[1] / 2.0) * cos(p[2] / 2.0) - cos(p[0] / 2.0) * sin(p[1] / 2.0) * sin(p[2] / 2.0);
	double y = cos(p[0] / 2.0) * sin(p[1] / 2.0) * cos(p[2] / 2.0) + sin(p[0] / 2.0) * cos(p[1] / 2.0) * sin(p[2] / 2.0);
	double z = cos(p[0] / 2.0) * cos(p[1] / 2.0) * sin(p[2] / 2.0) - sin(p[0] / 2.0) * sin(p[1] / 2.0) * cos(p[2] / 2.0);
	return dquat(w, x, y, z);
}

#endif
