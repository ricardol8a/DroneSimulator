//Headers
#include "register.hpp"

reg::reg(int length)
{
	data.reserve(19);
	target.reserve(length);
	status.reserve(length);
}

void reg::update(double time, dvec3 position, dvec3 attitude, dvec3 linearvelocity, dvec3 angularvelocity, dvec3 linearacceleration, dvec3 angularacceleration, bool flag)
{
	data.push_back(time);
	data.push_back(position.x);
	data.push_back(position.y);
	data.push_back(position.z);
	data.push_back(attitude.x);
	data.push_back(attitude.y);
	data.push_back(attitude.z);
	data.push_back(linearvelocity.x);
	data.push_back(linearvelocity.y);
	data.push_back(linearvelocity.z);
	data.push_back(angularvelocity.x);
	data.push_back(angularvelocity.y);
	data.push_back(angularvelocity.z);
	data.push_back(linearacceleration.x);
	data.push_back(linearacceleration.y);
	data.push_back(linearacceleration.z);
	data.push_back(angularacceleration.x);
	data.push_back(angularacceleration.y);
	data.push_back(angularacceleration.z);
	if (flag)
	{
		if (!status.empty() && (status.back()).front() == time)
			status.pop_back();
		status.push_back(data);
	}
	else
	{
		if (!target.empty() && (target.back()).front() == time)
			target.pop_back();
		target.push_back(data);
	}
	data.resize(0);
}

#if(VIDEO)
void reg::render(void)
{
	clr Color;
	glLineWidth(1.0);
	glColor4fv(Color.green);
	glBegin(GL_LINE_STRIP);
		for(unsigned int k = 0; k < target.size(); k++)
			glVertex3f(target[k][1], target[k][2], target[k][3]);
	glEnd();
	glColor4fv(Color.blue);
	glBegin(GL_LINE_STRIP);
		for(unsigned int k = 0; k < status.size(); k++)
			glVertex3f(status[k][1], status[k][2], status[k][3]);
	glEnd();
}
#endif
