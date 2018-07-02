//Headers
#include "camera.hpp"

#if(VIDEO)
cam::cam(dvec3 position, dvec3 boxcenter, dvec3 unitvector, dvec3 boxdimension)
{
	x = position;
	c = boxcenter;
	u = unitvector;
	d = boxdimension;
	b = false;
}

cam::cam(dvec3 position, dvec3 boxcenter, dvec3 unitvector, double angle, double ratio, double znear, double zfar)
{
	x = position;
	c = boxcenter;
	u = unitvector;
	d = dvec3(2.0 * znear * tan(angle / 2.0) * ratio, 2.0 * znear * tan(angle / 2.0), zfar - znear);
	b = true;
}

void cam::update(dvec3 position, dvec3 boxcenter, dvec3 unitvector, dvec3 boxdimension)
{
	x = position;
	c = boxcenter;
	u = unitvector;
	d = boxdimension;
}

void cam::update(dvec3 position, dvec3 boxcenter, dvec3 unitvector, double angle, double ratio, double znear, double zfar)
{
	x = position;
	c = boxcenter;
	u = unitvector;
	d = dvec3(2.0 * znear * tan(angle / 2.0) * ratio, 2.0 * znear * tan(angle / 2.0), zfar - znear);
}

void cam::render(void)
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	if(b)
		glFrustum(-d.x / 2.0, d.x / 2.0, -d.y / 2.0, d.y / 2.0, length(c - x) - d.z / 2.0, length(c - x) + d.z / 2.0);
	else
		glOrtho(-d.x / 2.0, d.x / 2.0, -d.y / 2.0, d.y / 2.0, length(c - x) - d.z / 2.0, length(c - x) + d.z / 2.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x.x, x.y, x.z, c.x, c.y, c.z, u.x, u.y, u.z);
}
#endif
