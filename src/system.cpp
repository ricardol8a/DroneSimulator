//Header
#include "system.hpp"

extern clk Clock;
extern vector<pth> Path;
extern vector<rbt> Robot;
extern vector<obs> Obstacle;
extern vector<reg> Register;
extern mdl Model;
#if(!MODEL)
extern ntf Output;
extern lnk Link;
#endif

clk Clock;
vector<pth> Path;
vector<ctl> Control;
vector<rbt> Robot;
vector<obs> Obstacle;
vector<reg> Register;
mdl Model;

sys::sys(cfg config)
{
	n = 0;
	state = false;
	dt = config.getValueDouble("defaulttranslation");
	dr = config.getValueDouble("defaultrotation") * (M_PI / 180.0);
	#if(!MODEL)
	k = 0;
	kmax = config.getValueInt("emergencybreak");
	#endif
	cfg settime(string(CONFIG) + string("time.cfg"));
	Clock = clk(settime);
	cfg setmodel(string(CONFIG) + string("model.cfg"));
	Model = mdl(setmodel);
	cfg setpath = cfg(string(CONFIG) + string("trajectory.cfg"));
	for (unsigned int i = 0; i != setpath.getTotalGroups(); i++)
		Path.push_back(pth(setpath.getGroup(string("path") + to_string(i))));
	cfg setrobot = cfg(string(CONFIG) + string("robot.cfg"));
	for (unsigned int i = 0; i != setrobot.getTotalGroups(); i++)
		Robot.push_back(rbt(setrobot.getGroup(string("robot") + to_string(i))));
	if(!Robot.size())
		throw "Error: There Are No Robots";
	if(Robot.size() != Path.size())
		throw "Error: Doesn't Match Robots and Paths";
	#if(!MODEL)
	if(Robot.size() != 1)
		throw "Error: There Are Many Robots";
	#endif
	cfg setcontrol = cfg(string(CONFIG) + string("control.cfg"));
	for (unsigned int i = 0; i != Robot.size(); i++)
	{
		Control.push_back(ctl(setcontrol));
		Control[i].handle(Robot[i].x, Robot[i].theta, Robot[i].v, Robot[i].omega);
	}
	cfg setobstacle = cfg(string(CONFIG) + string("obstacle.cfg"));
	for (unsigned int i = 0; i != setobstacle.getTotalGroups(); i++)
		Obstacle.push_back(obs(setobstacle.getGroup(string("obstacle") + to_string(i))));
	#if(!MODEL)
	Link.add(Robot[n].x, eul2qua(dvec3()), false, 2);
	#endif
	for (unsigned int i = 0; i != Robot.size(); i++)
	{
		Register.push_back(reg(static_cast<int>((Clock.tf - Clock.to) / Clock.ts) + 1));
		#if(CONTROL)
		Register.back().update(Clock.t, Path[i].x, Path[i].theta, Path[i].v, Path[i].omega, Path[i].a, Path[i].alpha, false);
		#endif
		Register.back().update(Clock.t, Robot[i].x, Robot[i].theta, Robot[i].v, Robot[i].omega, Robot[i].a, Robot[i].alpha, true);
	}
}

void sys::admin(void)
{
	#if(MODEL)
	if(Clock.t < Clock.tf)
		state = !state;
	else
		state = false;
	#else
	if(Clock.t > Clock.to)
		Output.socket0(2);
	else
		Output.socket0(1);
	#endif
}

void sys::manual(bool motion, bool sign, char axis)
{
	dvec3 dir(0.0), ref;
	#if(!CONTROL)
	dvec3 v = Control[n].v;
	dvec3 omega = Control[n].omega;
	#elif(MODEL)
	dvec3 x = Robot[n].x;
	dvec3 theta = Robot[n].theta;
	#endif
	if(motion)
		ref = dvec3(dt);
	else
		ref = dvec3(dr);
	dir[axis - 120] = (!sign - sign) * ref[axis - 120];
	if(motion)
	{
		#if(!CONTROL)
		v = v + dir;
		v = round(v / ref) * ref;		//Correction
		#elif(MODEL)
		x = x + dir;
		x = round(x / ref) * ref;		//Correction
		#endif
	}
	else
	{
		#if(!CONTROL)
		omega = omega + dir;
		omega = round(omega / ref) * ref;	//Correction
		#elif(MODEL)
		theta = ang(theta + dir);
		theta = round(theta / ref) * ref;	//Correction
		#endif
	}
	#if(!CONTROL)
	Control[n].handle(Control[n].x, Control[n].theta, v, omega);
	#elif(MODEL)
	Robot[n].handle(x, theta);
	Control[n].handle(Robot[n].x, Robot[n].theta, Control[n].v, Control[n].omega);
	//Manage Collisions between Objects
	Register[n].update(Clock.t, Robot[n].x, Robot[n].theta, Robot[n].v, Robot[n].omega, Robot[n].a, Robot[n].alpha, true);
	#endif
}

void sys::automatic(void)
{
	#if(MODEL)
	if(state)
	{
		state = Clock.update();
		#if(CONTROL)
		for(unsigned int i = 0; i < Path.size(); i++)
			Path[i].update();
		for(unsigned int i = 0; i < Register.size(); i++)
			Register[i].update(Clock.t, Path[i].x, Path[i].theta, Path[i].v, Path[i].omega, Path[i].a, Path[i].alpha, false);
		for(unsigned int i = 0; i < Control.size(); i++)
			Control[i].update(Path[i].x, Path[i].theta, Path[i].v, Path[i].omega, Path[i].a, Path[i].alpha, Robot[i].x, Robot[i].theta, Robot[i].v, Robot[i].omega, Robot[i].a, Robot[i].alpha);
		#else
		for(unsigned int i = 0; i < Control.size(); i++)
			Control[i].update(Robot[i].x, Robot[i].theta);
		#endif
		for(unsigned int i = 0; i < Robot.size(); i++)
			Robot[i].update(Control[i].x, Control[i].theta, Control[i].v, Control[i].omega, Control[i].fb, Control[i].taob, Control[i].w);
		//Manage Collisions with Robot.handle()
		for(unsigned int i = 0; i < Register.size(); i++)
			Register[i].update(Clock.t, Robot[i].x, Robot[i].theta, Robot[i].v, Robot[i].omega, Robot[i].a, Robot[i].alpha, true);
		Model.update();
	}
	#else
	if(state && Clock.update())
	{
		#if(CONTROL)
		Path[n].update();
		Register[n].update(Clock.t, Path[n].x, Path[n].theta, Path[n].v, Path[n].omega, Path[n].a, Path[n].alpha, false);
		Control[n].update(Path[n].x, Path[n].theta, Path[n].v, Path[n].omega, Path[n].a, Path[n].alpha, Robot[n].x, Robot[n].theta, Robot[n].v, Robot[n].omega, Robot[n].a, Robot[n].alpha);
		#else
		Control[n].update(Robot[n].x, Robot[n].theta);
		#endif
		Output.socket1(dvec4(Control[n].theta.y / Model.thetamax.y, -Control[n].theta.x / Model.thetamax.x, Control[n].v.z / Model.vmax.z, Control[n].omega.z / Model.omegamax.z));
	}
	else
		Output.socket1(dvec4(0.0, 0.0, 0.0, 0.0));
	#endif
}

void sys::focus(int number)
{
	if(number < static_cast<int>(Robot.size()))
		n = static_cast<unsigned int>(number);
}

#if(!MODEL)
void sys::assign(bool status, dvec3 position, dquat orientation)
{
	state = status;
	if(state)
	{
		dvec3 attitude = ang(qua2eul(orientation));
		#if(CONTROL)
		if((position == Robot[n].x) && (attitude == Robot[n].theta))
			k++;
		else
			k = 0;
		state = (k < kmax);
		#endif
		if(Clock.t > Clock.to)
			Robot[n].update(position, attitude);
		else
		{
			Robot[n].handle(position, attitude);
			Control[n].handle(Robot[n].x, Robot[n].theta, Control[n].v, Control[n].omega);
		}
		Register[n].update(Clock.t, Robot[n].x, Robot[n].theta, Robot[n].v, Robot[n].omega, Robot[n].a, Robot[n].alpha, true);
	}
}
#endif
