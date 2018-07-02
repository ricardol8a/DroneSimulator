//Header
#include "scene.hpp"

#if(VIDEO)
extern win Window;
extern txt Info;
extern vector<pth> Path;
extern vector<obs> Obstacle;
extern vector<rbt> Robot;
extern vector<reg> Register;
extern spc Space;
extern pln Plane;
extern vector<box> Sector;

spc Space;
pln Plane;
vector<box> Sector;
vector<cam> Camera;

vector<string> coordenates = {"rectangular", "cylindrical", "spherical"};
vector<string> eyesights = {"first", "second", "third"};

dvec3 direction[3][3][3] = {{{dvec3(-1.0, 0.0, 0.0), dvec3(0.0, 0.0, 1.0), dvec3(0.0, 1.0, 0.0)}, {dvec3(0.0, -1.0, 0.0), dvec3(0.0, 0.0, 1.0), dvec3(-1.0, 0.0, 0.0)}, {dvec3(0.0, 0.0, -1.0), dvec3(0.0, 1.0, 0.0), dvec3(1.0, 0.0, 0.0)}}, {{dvec3(-1.0, 0.0, 0.0), dvec3(0.0, 0.0, 1.0), dvec3(0.0, 1.0, 0.0)}, {dvec3(0.0, 1.0, 0.0), dvec3(0.0, 0.0, 1.0), dvec3(1.0, 0.0, 0.0)}, {dvec3(0.0, 0.0, -1.0), dvec3(-1.0, 0.0, 0.0), dvec3(0.0, 1.0, 0.0)}}, {{dvec3(-1.0, 0.0, 0.0), dvec3(0.0, -1.0, 0.0), dvec3(0.0, 0.0, 1.0)}, {dvec3(0.0, 1.0, 0.0), dvec3(-1.0, 0.0, 0.0), dvec3(0.0, 0.0, 1.0)}, {dvec3(0.0, 0.0, 1.0), dvec3(0.0, -1.0, 0.0), dvec3(1.0, 0.0, 0.0)}}};

GLdouble boundary[6][4] = {{ 0.0, 0.0, -1.0, 1.0 }, { 0.0, 0.0, 1.0, 1.0 }, { 0.0, -1.0, 0.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 }, { -1.0, 0.0, 0.0, 1.0 }, { 1.0, 0.0, 0.0, 1.0 }};

scn::scn(cfg config, cfg config2D, cfg config3D)
{
	Sector.reserve(5);
	Sector.push_back(box(ivec2(0), ivec2(config2D.getValueVectorInt("size", 1)), ivec2(config2D.getValueVectorInt("size", 0), config2D.getValueVectorInt("size", 1))));
	Sector.push_back(box(ivec2((3 * config2D.getValueVectorInt("size", 0) - config2D.getValueVectorInt("size", 1)) / 3, 0), ivec2(config2D.getValueVectorInt("size", 1) / 3), ivec2(config2D.getValueVectorInt("size", 0), config2D.getValueVectorInt("size", 1))));
	Sector.push_back(box(ivec2((3 * config2D.getValueVectorInt("size", 0) - config2D.getValueVectorInt("size", 1)) / 3, config2D.getValueVectorInt("size", 1) / 3), ivec2(config2D.getValueVectorInt("size", 1) / 3), ivec2(config2D.getValueVectorInt("size", 0), config2D.getValueVectorInt("size", 1))));
	Sector.push_back(box(ivec2((3 * config2D.getValueVectorInt("size", 0) - config2D.getValueVectorInt("size", 1)) / 3, 2 * config2D.getValueVectorInt("size", 1) / 3), ivec2(config2D.getValueVectorInt("size", 1) / 3), ivec2(config2D.getValueVectorInt("size", 0), config2D.getValueVectorInt("size", 1))));
	Sector.push_back(box(ivec2(0.0), ivec2(config2D.getValueVectorInt("size", 0), config2D.getValueVectorInt("size", 1)), ivec2(config2D.getValueVectorInt("size", 0), config2D.getValueVectorInt("size", 1))));
	Space = spc(dvec3(0.0), config3D.getValueDouble("size"), config3D.getValueDouble("maxsize"));
	Plane = pln(ivec2(0.0), ivec2(config2D.getValueVectorInt("size", 0), config2D.getValueVectorInt("size", 1)));
	for(unsigned int i = 0; i != 6; i++)
		boundary[i][3] = copysign(Space.d / 2.0, boundary[i][3]);
	type = find(coordenates.begin(), coordenates.end(), config.getValueString("coordenate")) - coordenates.begin();
	if(type == coordenates.size())
		throw "Error: Unknow Coordinate Type";
	view = find(eyesights.begin(), eyesights.end(), config.getValueString("eyesight")) - eyesights.begin();
	if(view == eyesights.size())
		throw "Error: Unknow Eyesight Type";
	if(((type == 1) && (view == 1)) || ((type == 2) && (view != 0)))
		throw "Error: Conflict Coordinate Eyesight";
	angle = 45.0 * (M_PI / 180.0);
	near = (1.0 + sqrt(5.0)) / 2.0;
	p = dvec3(config.getValueVectorDouble("position", 0), config.getValueVectorDouble("position", 1), config.getValueVectorDouble("position", 2)) * dvec3(sqrt(static_cast<double>(type + 1)), (type != 0) * (M_PI / 180.0) + (type == 0), (type == 2) * (M_PI / 180.0) + (type != 2)) - near * direction[type][view][0];
	far = p[view] + sqrt(static_cast<double>(type * (!view) + 1)) * Space.d / 2.0;
	if (far < near)
		far = near;
	dvec3 x = p;
	dvec3 u = direction[type][view][1];
	dvec3 c = direction[type][view][0];
	switch(type)
	{
		case 0:
			break;
		case 1:
			x = cyl2car(x);
			u = cyl2car(u, p, false);
			c = cyl2car(c, p, false);
			break;
		case 2:
			x = sph2car(p);
			u = sph2car(u, p, false);
			c = sph2car(c, p, false);
			break;
	}
	c = c * (far + near) / 2.0 + x;
	ratio = static_cast<double>(Sector[0].d.x) / static_cast<double>(Sector[0].d.y);
	Camera.reserve(Sector.size());
	Camera.push_back(cam(x, c, u, angle, ratio, near, far));
	Camera.push_back(cam(dvec3(0.0, 0.0, config3D.getValueDouble("size") / 2.0), dvec3(0.0), dvec3(0.0, 1.0, 0.0), dvec3(config3D.getValueDouble("size"))));
	Camera.push_back(cam(dvec3(0.0, -config3D.getValueDouble("size") / 2.0, 0.0), dvec3(0.0), dvec3(0.0, 0.0, 1.0), dvec3(config3D.getValueDouble("size"))));
	Camera.push_back(cam(dvec3(-config3D.getValueDouble("size") / 2.0, 0.0, 0.0), dvec3(0.0), dvec3(0.0, 0.0, 1.0), dvec3(config3D.getValueDouble("size"))));
	Camera.push_back(cam(dvec3(0.0, 0.0, 1.0), dvec3(0.0), dvec3(0.0, 1.0, 0.0), dvec3(dvec2(config2D.getValueVectorInt("size", 0), config2D.getValueVectorInt("size", 1)), 2.0)));
	dt = config.getValueDouble("defaultmove");
	dr = config.getValueDouble("defaultturn") * (M_PI / 180.0);
}

void scn::shift(void)
{
	p = p + near * direction[type][view][0];
	view = view + (type != 2) * (1 + (type == 1));
	if (view > 2)
	{
		view = 0;
		//type = type + 1;
		//if (type > 2)
			//type = 0;
	}
	//dvec3 ref = dvec3(sqrt(static_cast<double>(type + 1)) * dt, (type != 0) * dr + (type == 0) * dt, (type == 2) * dr + (type != 2) * dt);
	//p = round(p / ref) * ref;
	p = p - near * direction[type][view][0];
		far = p[view] + sqrt(static_cast<double>(type * (!view) + 1)) * Space.d / 2.0;
	if (far < near)
		far = near;
	dvec3 x = p;
	dvec3 u = direction[type][view][1];
	dvec3 c = direction[type][view][0];
	switch(type)
	{
		case 0:
			break;
		case 1:
			x = cyl2car(x);
			u = cyl2car(u, p, false);
			c = cyl2car(c, p, false);
			break;
		case 2:
			x = sph2car(p);
			u = sph2car(u, p, false);
			c = sph2car(c, p, false);
			break;
	}
	c = c * (far + near) / 2.0 + x;
	Camera[0].update(x, c, u, angle, ratio, near, far);
}

void scn::move(bool sign, char axis)
{
	dvec3 ref = dvec3(sqrt(static_cast<double>(type + 1)) * dt, (type != 0) * dr + (type == 0) * dt, (type == 2) * dr + (type != 2) * dt);
	dvec3 lowlim(-INFINITY);
	dvec3 uplim(INFINITY);
	p = p + near * direction[type][view][0];
	if(type)
	{
		lowlim[0] = 0.0;
		if(type == 2)
		{
			lowlim[1] = 0.0;
			uplim[1] = M_PI;
		}
	}
	p = clamp(p + static_cast<double>(!sign - sign) * ref * direction[type][view][axis - 120], lowlim, uplim);
	p = round(p / ref) * ref;			//Correction
	p = p - near * direction[type][view][0];
	far = p[view] + sqrt(static_cast<double>(type * (!view) + 1)) * Space.d / 2.0;
	if (far < near)
		far = near;
	dvec3 x = p;
	dvec3 u = direction[type][view][1];
	dvec3 c = direction[type][view][0];
	switch(type)
	{
		case 0:
			break;
		case 1:
			x = cyl2car(x);
			u = cyl2car(u, p, false);
			c = cyl2car(c, p, false);
			break;
		case 2:
			x = sph2car(p);
			u = sph2car(u, p, false);
			c = sph2car(c, p, false);
			break;
	}
	c = c * (far + near) / 2.0 + x;
	Camera[0].update(x, c, u, angle, ratio, near, far);
}

void scn::update(void)
{
	double aux = 0;
	for(unsigned int i = 0; i < Robot.size(); i++)
		aux = fmax(2.0 * compMax(abs(Robot[i].x - Space.x) + Robot[i].r), aux);
	if(aux > Space.d)
	{
		Space.update(aux);
		for(unsigned int i = 0; i != 6; i++)
			boundary[i][3] = copysign(Space.d / 2.0, boundary[i][3]);
		far = p[view] + sqrt(static_cast<double>(type * (!view) + 1)) * Space.d / 2.0;
		if (far < near)
			far = near;
		dvec3 c = direction[type][view][0];
		switch(type)
		{
			case 0:
				break;
			case 1:
				c = cyl2car(c, p, false);
				break;
			case 2:
				c = sph2car(c, p, false);
				break;
		}
		c = c * (far + near) / 2.0;
		Camera[0].update(Camera[0].x, c + Camera[0].x, Camera[0].u, angle, ratio, near, far);
		Camera[1].update(normalize(Camera[1].x - Camera[1].c) * Space.d, Camera[1].c, Camera[1].u, dvec3(Space.d));
		Camera[2].update(normalize(Camera[2].x - Camera[2].c) * Space.d, Camera[2].c, Camera[2].u, dvec3(Space.d));
		Camera[3].update(normalize(Camera[3].x - Camera[2].c) * Space.d, Camera[3].c, Camera[3].u, dvec3(Space.d));
		Space.build();
	}
	if(Plane.d != Window.d)
	{
		Plane.update(Window.d);
		for (unsigned int i = 0; i < Sector.size(); i++)
			Sector[i].update(Plane.d);
		ratio = static_cast<double>(Sector[0].d.x) / static_cast<double>(Sector[0].d.y);
		Camera[0].update(Camera[0].x, Camera[0].c, Camera[0].u, angle, ratio, near, far);
		Camera[4].update(normalize(Camera[4].x - Camera[4].c) * dvec3(Plane.d, 2.0), Camera[4].c, Camera[4].u, dvec3(Plane.d, 2.0));
		Plane.build();
	}
}

void scn::build(void)
{
	Space.build();
	for(unsigned int i = 0; i < Path.size(); i++)
		Path[i].build();
	for(unsigned int i = 0; i < Robot.size(); i++)
		Robot[i].build();
	for(unsigned int i = 0; i < Obstacle.size(); i++)
		Obstacle[i].build();
	Plane.build();
}

void scn::render(void)
{
	for(unsigned int i = 0; i < Sector.size(); i++)
	{
		Sector[i].render();
		Camera[i].render();
		if(Sector.size() - i - 1)
		{
			Space.render();
			glClipPlane (GL_CLIP_PLANE0, boundary[0]);
			glClipPlane (GL_CLIP_PLANE1, boundary[1]);
			glClipPlane (GL_CLIP_PLANE2, boundary[2]);
			glClipPlane (GL_CLIP_PLANE3, boundary[3]);
			glClipPlane (GL_CLIP_PLANE4, boundary[4]);
			glClipPlane (GL_CLIP_PLANE5, boundary[5]);
			glEnable (GL_CLIP_PLANE0);
			glEnable (GL_CLIP_PLANE1);
			glEnable (GL_CLIP_PLANE2);
			glEnable (GL_CLIP_PLANE3);
			glEnable (GL_CLIP_PLANE4);
			glEnable (GL_CLIP_PLANE5);
			for(unsigned int j = 0; j < Path.size(); j++)
				Path[j].render();
			for(unsigned int j = 0; j < Robot.size(); j++)
				Robot[j].render();
			for(unsigned int j = 0; j < Obstacle.size(); j++)
				Obstacle[j].render();
			for(unsigned int j = 0; j < Register.size(); j++)
				Register[j].render();
			glDisable (GL_CLIP_PLANE0);
			glDisable (GL_CLIP_PLANE1);
			glDisable (GL_CLIP_PLANE2);
			glDisable (GL_CLIP_PLANE3);
			glDisable (GL_CLIP_PLANE4);
			glDisable (GL_CLIP_PLANE5);
		}
		else
		{
			Plane.render();
			Info.render();
		}
	}
}
#endif
