//Headers
#include "trajectory.hpp"

extern clk Clock;

vector<string> curves = {"point", "line", "parabole", "wave", "zigzag", "spiral", "circle", "lemniscate", "peanut", "heart", "fish", "star", "limacon", "rose", "polygon", "helix", "swirl", "ring", "crown", "torus", "lissajous"};

pth::pth(cfg config)
{
	type = find(curves.begin(), curves.end(), config.getValueString("curve")) - curves.begin();
	if(type == curves.size())
		throw "Error: Unknow Trajectory Type";
	c = dvec3(config.getValueVectorDouble("translate", 0), config.getValueVectorDouble("translate", 1), config.getValueVectorDouble("translate", 2));
	d = dvec3(config.getValueVectorDouble("scale", 0), config.getValueVectorDouble("scale", 1), config.getValueVectorDouble("scale", 2));
	n = normalize(dvec3(config.getValueVectorDouble("rotatevector", 0), config.getValueVectorDouble("rotatevector", 1), config.getValueVectorDouble("rotatevector", 2)));
	angle = config.getValueDouble("rotateangle") * (M_PI / 180.0);
	R = (outerProduct(n, n) + (cos(angle) * (dmat3(1.0) - outerProduct(n, n))) + (sin(angle) * dmat3(0.0, n.z, -n.y, -n.z, 0.0, n.x, n.y, -n.x, 0.0)));
	l = static_cast<double>(config.getValueInt("sections"));
	b = config.getValueDouble("bias") - static_cast<int>(config.getValueDouble("bias"));
	k = config.getValueDouble("turns");
	f = config.getValueBool("compass");
	g = config.getValueDouble("gap") * (M_PI / 180.0);
	x = c + R * (d * function());
	theta = ang(dvec3(0.0, 0.0, !f * (M_PI / 2.0) + g));
	v = dvec3(0.0);
	omega = dvec3(0.0);
	a = dvec3(0.0);
	alpha = dvec3(0.0);
}

dvec3 pth::function(void)
{
	double t;
	if (((type > 0) && (type < 6)) || ((type > 14) && (type < 17)))
		t = abs(2.0 * (0.5 * k * (Clock.t - Clock.to) / (Clock.tf - Clock.to) - floor(0.5 * k * (Clock.t - Clock.to) / (Clock.tf - Clock.to) + 0.5))) + b;
	else
		t = k * (Clock.t - Clock.to) / (Clock.tf - Clock.to) - floor(k * (Clock.t - Clock.to) / (Clock.tf - Clock.to)) + b;
	switch (type)
	{
		case 0 :
			return dvec3(0.0);
		case 1 :
			return line(t);
		case 2 :
			return parabole(t);
		case 3 :
			return wave(l, t);
		case 4 :
			return zigzag(l, t);
		case 5 :
			return spiral(l, t);
		case 6 :
			return circle(t);
		case 7 :
			return lemniscate(t);
		case 8 :
			return peanut(t);
		case 9 :
			return heart(t);
		case 10:
			return fish(t);
		case 11:
			return star(t);
		case 12:
			return limacon(t);
		case 13:
			return rose(l, t);
		case 14:
			return polygon(l, t);
		case 15:
			return helix(l, t);
		case 16:
			return swirl(l, t);
		case 17:
			return ring(l, t);
		case 18:
			return crown(l, t);
		case 19:
			return torus(l, t);
		case 20:
			return lissajous(t);
		default:
			return dvec3(0.0);
	}
}

void pth::update(void)
{
	double dt = Clock.ts;
	dvec3 xo = x;
	dvec3 thetao = theta;
	x = c + R * (d * function());
	dvec3 vo = v;
	v = (x - xo) / dt;
	a = (v - vo) / dt;
	if(dvec2(v) != dvec2(0.0))
		theta = ang(dvec3(0.0, 0.0, !f * ((car2cyl(dvec3(dvec2(v), 0.0)))[1]) + g));
	dvec3 omegao = omega;
	omega = (dvec3(lessThan(theta - thetao, dvec3(-M_PI))) * (theta + dvec3(2.0 * M_PI)) + dvec3(greaterThan(theta - thetao, dvec3(-M_PI))) * dvec3(lessThan(theta - thetao, dvec3(M_PI))) * theta + dvec3(greaterThan(theta - thetao, dvec3(M_PI))) * (theta - dvec3(2.0 * M_PI)) - thetao) / dt;
	alpha = (omega - omegao) / dt;
}

#if (VIDEO)
void pth::build(void)
{
	dvec3 xo = x;
	clk Clocko = Clock;
	clr Color;
	list = glGenLists(1);
	glNewList(list, GL_COMPILE);
		glColor4fv(Color.lime);
		glLineWidth(1.0);
	   	glBegin(GL_LINE_STRIP);
	   		while(Clock.update())
	   		{
				x = c + R * (d * function());
				glVertex3f(x.x, x.y, x.z);
			}
		glEnd();
	glEndList();
	Clock = Clocko;
	x = xo;
}

void pth::render(void)
{
	glPushMatrix();
		glCallList(list);
	glPopMatrix();
}
#endif
