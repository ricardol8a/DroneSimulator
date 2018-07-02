//Header
#include "control.hpp"

extern clk Clock;
extern mdl Model;

ctl::ctl(cfg config)
{
	if (Model.level)
	{
		Controller.push_back(ctr(config.getGroup("controllerposition")));
		Controller.push_back(ctr(config.getGroup("controllerattitude")));
	}
	x = dvec3();
	theta = dvec3();
	v = dvec3();
	omega = dvec3();
	a = dvec3();
	alpha = dvec3();
	fb = dvec3();
	taob = dvec3();
	w = dvec4();
	e = dvec3();
	ie = dvec3();
	de = dvec3();
	epsilon = dvec3();
	iepsilon = dvec3();
	depsilon = dvec3();
	if(Model.xsigma)
	{
		Filter.push_back(ftr(config.getGroup("filterposition")));
		Filter[0].handle(x, x);
		if(!Filter[0].type)
			esigma = dvec3(Model.xsigma);
	}
	if(Model.thetasigma)
	{
		Filter.push_back(ftr(config.getGroup("filterattitude")));
		Filter[1].handle(theta, theta);
		if(!Filter[1].type)
			epsilonsigma = dvec3(Model.thetasigma);
	}
}
#if(CONTROL)
void ctl::update(dvec3 positionideal, dvec3 attitudeideal, dvec3 linearvelocityideal, dvec3 angularvelocityideal, dvec3 linearaccelerationideal, dvec3 angularaccelerationideal, dvec3 positionreal, dvec3 attitudereal, dvec3 linearvelocityreal, dvec3 angularvelocityreal, dvec3 linearaccelerationreal, dvec3 angularaccelerationreal)
{
	double dt = Clock.ts;
	dvec3 xi = positionideal;
	dvec3 thetai = attitudeideal;
	dvec3 vi = linearvelocityideal;
	dvec3 omegai = angularvelocityideal;
	dvec3 ai = linearaccelerationideal;
	dvec3 alphai = angularaccelerationideal;
	dvec3 xo = positionreal;
	dvec3 thetao = attitudereal;
	dvec3 vo = linearvelocityreal;
	dvec3 omegao = angularvelocityreal;
	dvec3 ao = linearaccelerationreal;
	dvec3 alphao = angularaccelerationreal;
	if(Model.xsigma)
	{
		if(!Filter[0].type)
		{
			Filter[0].update(esigma / (dvec3(Model.xsigma) + esigma), dvec3(Model.xsigma) / (dvec3(Model.xsigma) + esigma));
			esigma = esigma * (1.0 - esigma / (Model.xsigma + esigma));
			#if(!MODEL)//OJO
			esigma += dvec3(Model.xsigma);
			#endif
		}
		xo = Filter[0].calcule(xo, x);
	}
	if(Model.thetasigma)
	{
		if(!Filter[1].type)
		{
			Filter[1].update(epsilonsigma / (dvec3(Model.thetasigma) + epsilonsigma), dvec3(Model.thetasigma) / (dvec3(Model.thetasigma) + epsilonsigma));
			epsilonsigma = epsilonsigma * (1.0 - epsilonsigma / (Model.thetasigma + epsilonsigma));
			#if(!MODEL)//OJO
			epsilonsigma += dvec3(Model.thetasigma);
			#endif
		}
		thetao = ang(Filter[1].calcule(thetao, theta));
	}
	x = xi;
	theta = thetai;
	if (Model.level)
	{
		x = clamp(x, -Model.xmax, Model.xmax);
		dvec3 eo = e;
		e = x - xo;
		ie += e * dt;
		#if(CONTROL == 1)
		de = (e - eo) / dt;
		#elif(CONTROL == 2)
		de = ((x - xo) / dt) - vo;
		#elif(CONTROL == 3)
		de = -vo;
		#else
		de = vi - vo;
		#endif
		a = Controller[0].calcule(e, ie, de);
		//a = ai + Controller[0].calcule(e, ie, de);
		a = clamp(a, (max(-Model.vmax, (-Model.xmax - xo) / dt) - vo) / dt, (min(Model.vmax, (Model.xmax - xo) / dt) - vo) / dt);
		v = vo + a * dt;
		x = xo + v * dt;
		theta = dvec3(atan(dvec2((sin(thetao.z) * a.x - cos(thetao.z) * a.y) / sqrt(pow(cos(thetao.z) * a.x + sin(thetao.z) * a.y, 2.0) + pow(a.z + Model.g, 2.0)) , (cos(thetao.z) * a.x + sin(thetao.z) * a.y) / (a.z + Model.g))), theta.z);
		theta = clamp(theta, -Model.thetamax, Model.thetamax);
		dvec3 epsilono = epsilon;
		epsilon = dvec3(lessThan(theta - thetao, dvec3(-M_PI))) * (theta + dvec3(2.0 * M_PI)) + dvec3(greaterThan(theta - thetao, dvec3(-M_PI))) * dvec3(lessThan(theta - thetao, dvec3(M_PI))) * theta + dvec3(greaterThan(theta - thetao, dvec3(M_PI))) * (theta - dvec3(2.0 * M_PI)) - thetao;
		iepsilon += epsilon * dt;
		#if(CONTROL == 1)
		depsilon = (epsilon - epsilono) / dt;
		#elif(CONTROL == 2)
		depsilon = (dvec3(lessThan(theta - thetao, dvec3(-M_PI))) * (theta + dvec3(2.0 * M_PI)) + dvec3(greaterThan(theta - thetao, dvec3(-M_PI))) * dvec3(lessThan(theta - thetao, dvec3(M_PI))) * theta + dvec3(greaterThan(theta - thetao, dvec3(M_PI))) * (theta - dvec3(2.0 * M_PI)) - thetao) / dt - omegao;
		#elif(CONTROL == 3)
		depsilon = -omegao;
		#else
		depsilon = omegai - omegao;
		#endif
		alpha = Controller[1].calcule(epsilon, iepsilon, depsilon);
		//alpha = alphai + Controller[1].calcule(epsilon, iepsilon, depsilon);
		#if(!MODEL)
		alpha = dvec3(((dvec2(lessThan(dvec2(theta - thetao), dvec2(-M_PI))) * (dvec2(theta) + dvec2(2.0 * M_PI)) + dvec2(greaterThan(dvec2(theta - thetao), dvec2(-M_PI))) * dvec2(lessThan(dvec2(theta - thetao), dvec2(M_PI))) * dvec2(theta) + dvec2(greaterThan(dvec2(theta - thetao), dvec2(M_PI))) * (dvec2(theta) - dvec2(2.0 * M_PI)) - dvec2(thetao)) / dt - dvec2(omegao)) / dt, alpha.z);
        	#endif
		alpha = clamp(alpha, (max(-Model.omegamax, (-Model.thetamax - thetao) / dt) - omegao) / dt, (min(Model.omegamax, (Model.thetamax - thetao) / dt) - omegao) / dt);
		omega = omegao + alpha * dt;
		theta = ang(thetao + omega * dt);
		a = dvec3((cos(theta.z) * sin(theta.y) * cos(theta.x) + sin(theta.z) * sin(theta.x)) * (a.z + Model.g) / (cos(theta.x) * cos(theta.y)), (sin(theta.z) * sin(theta.y) * cos(theta.x) - cos(theta.z) * sin(theta.x)) * (a.z + Model.g) / (cos(theta.x) * cos(theta.y)), a.z);
		v = vo + a * dt;
		x = xo + v * dt;
		#if(!MODEL)
		v = a * dt;
		omega = alpha * dt;
		#endif
		if (Model.level > 1)
		{
			fb = Model.m * dvec3(0.0, 0.0, (a.z + Model.g)) / (cos(theta.x) * cos(theta.y));
			//dmat3 J = dmat3(1.0, 0.0, -sin(theta.y), 0.0, cos(theta.x), cos(theta.y) * sin(theta.x), 0.0, -sin(theta.x), cos(theta.y) * cos(theta.x)) * Model.I * dmat3(1.0, 0.0, 0.0, 0.0, cos(theta.x), -sin(theta.x), -sin(theta.y), cos(theta.y) * sin(theta.x), cos(theta.y) * cos(theta.x));
			//taob = J * alpha;
			taob = Model.I * alpha;
			dvec2 aux;
			fb = dvec3(0.0, 0.0, clamp(fb.z, 4.0 * Model.kl * Model.w2min, 4.0 * Model.kl * Model.w2max));
			taob = clamp(taob, dvec3(sqrt(2.0) * Model.l * Model.kl * dvec2(Model.w2min - Model.w2max), 2.0 * Model.kd * (Model.w2min - Model.w2max)), dvec3(sqrt(2.0) * Model.l * Model.kl * dvec2(Model.w2max - Model.w2min), 2.0 * Model.kd * (Model.w2max - Model.w2min)));
			aux = clamp(dvec2(fb.z / Model.kl + sqrt(2.0) * taob.x / (Model.l * Model.kl), fb.z / Model.kl - sqrt(2.0) * taob.x / (Model.l * Model.kl)), dvec2(4.0 * Model.w2min), dvec2(4.0 * Model.w2max));
			fb.z = Model.kl * (aux[0] + aux[1]) / 2.0;
			taob.x = sqrt(2.0) * Model.l * Model.kl * (aux[0] - aux[1]) / 4.0;
			aux = clamp(dvec2(fb.z / Model.kl + sqrt(2.0) * taob.y / (Model.l * Model.kl), fb.z / Model.kl - sqrt(2.0) * taob.y / (Model.l * Model.kl)), dvec2(4.0 * Model.w2min), dvec2(4.0 * Model.w2max));
			fb.z = Model.kl * (aux[0] + aux[1]) / 2.0;
			taob.y = sqrt(2.0) * Model.l * Model.kl * (aux[0] - aux[1]) / 4.0;
			aux = clamp(dvec2(fb.z / Model.kl + taob.z / Model.kd, fb.z / Model.kl - taob.z / Model.kd), dvec2(4.0 * Model.w2min), dvec2(4.0 * Model.w2max));
			fb.z = Model.kl * (aux[0] + aux[1]) / 2.0;
			taob.z = Model.kd * (aux[0] - aux[1]) / 2.0;
			aux = clamp(dvec2(sqrt(2.0) * (taob.x + taob.y) / (Model.l * Model.kl), sqrt(2.0) * (taob.x - taob.y) / (Model.l * Model.kl)), dvec2(2.0 * (Model.w2min - Model.w2max)), dvec2(2.0 * (Model.w2max - Model.w2min)));
			taob.x = sqrt(2.0) * Model.l * Model.kl * (aux[0] + aux[1]) / 4.0;
			taob.y = sqrt(2.0) * Model.l * Model.kl * (aux[0] - aux[1]) / 4.0;
			aux = clamp(dvec2(sqrt(2.0) * taob.x / (Model.l * Model.kl) + taob.z / Model.kd, sqrt(2.0) * taob.x / (Model.l * Model.kl) - taob.z / Model.kd), dvec2(2.0 * (Model.w2min - Model.w2max)), dvec2(2.0 * (Model.w2max - Model.w2min)));
			taob.x = sqrt(2.0) * Model.l * Model.kl * (aux[0] + aux[1]) / 4.0;
			taob.z = Model.kd * (aux[0] - aux[1]) / 2.0;
			aux = clamp(dvec2(taob.y / (Model.l * Model.kl) + taob.z / Model.kd, taob.y / (Model.l * Model.kl) - taob.z / Model.kd), dvec2(2.0 * (Model.w2min - Model.w2max)), dvec2(2.0 * (Model.w2max - Model.w2min)));
			taob.y = sqrt(2.0) * Model.l * Model.kl * (aux[0] + aux[1]) / 4.0;
			taob.z = Model.kd * (aux[0] - aux[1]) / 2.0;
			if (Model.level > 2)
			{
				dvec4 w2;
				w2 = inverse(Model.M) * dvec4(fb.z, taob);
				w2 = clamp(w2, dvec4(Model.w2min), dvec4(Model.w2max));
				w = sqrt(w2);
				fb = dvec3(0.0, 0.0, Model.kl *(w2[3] + w2[2] + w2[1] + w2[0]));
				taob = dvec3(sqrt(2.0) / 2 * Model.l * Model.kl * (w2[3] + w2[2] - w2[1] - w2[0]), sqrt(2.0) / 2 * Model.l * Model.kl * (- w2[3] + w2[2] + w2[1] - w2[0]), Model.kd * (w2[3] - w2[2] + w2[1] - w2[0]));
			}
			alpha = inverse(Model.I) * taob;
			omega = omegao + alpha * dt;
			theta = ang(thetao + omega * dt);
			dmat3 R = dmat3(cos(theta.z) * cos(theta.y), sin(theta.z) * cos(theta.y), -sin(theta.y), cos(theta.z) * sin(theta.y) * sin(theta.x) - sin(theta.z) * cos(theta.x), sin(theta.z) * sin(theta.y) * sin(theta.x) + cos(theta.z) * cos(theta.x), cos(theta.y) * sin(theta.x), cos(theta.z) * sin(theta.y) * cos(theta.x) + sin(theta.z) * sin(theta.x), sin(theta.z) * sin(theta.y) * cos(theta.x) - cos(theta.z) * sin(theta.x), cos(theta.y) * cos(theta.x));
			a = R * fb / Model.m - dvec3(0.0, 0.0, Model.g);
			v = vo + a * dt;
			x = xo + v * dt;
		}
	}
}
#else
void ctl::update(dvec3 position, dvec3 attitude)
{
	double dt = Clock.ts;
	dvec3 xo = position;
	dvec3 thetao = attitude;
	dvec3 vo = v;
	dvec3 omegao = omega;
	v = clamp(v, max(-Model.vmax, (-Model.xmax - xo) / dt), min(Model.vmax, (Model.xmax - xo) / dt));
	x = xo + v * dt;
	//a = (v - vo) / dt;
	//theta = dvec3(atan(dvec2((sin(attitude.z) * a.x - cos(attitude.z) * a.y) / sqrt(pow(cos(attitude.z) * a.x + sin(attitude.z) * a.y, 2.0) + pow(a.z + Model.g, 2.0)) , (cos(attitude.z) * a.x + sin(attitude.z) * a.y) / (a.z + Model.g))), attitude.z - (2.0 * M_PI) * roundEven(attitude.z / (2.0 * M_PI)) + omega.z * dt);
	theta = dvec3(atan(dvec2((sin(thetao.z) * v.x - cos(thetao.z) * v.y) / sqrt(pow(cos(thetao.z) * v.x + sin(thetao.z) * v.y, 2.0) + pow(v.z + Model.g, 2.0)) , (cos(thetao.z) * v.x + sin(thetao.z) * v.y) / (v.z + Model.g))), thetao.z + omegao.z * dt);
	omega = (dvec3(lessThan(theta - thetao, dvec3(-M_PI))) * (theta + dvec3(2.0 * M_PI)) + dvec3(greaterThan(theta - thetao, dvec3(-M_PI))) * dvec3(lessThan(theta - thetao, dvec3(M_PI))) * theta + dvec3(greaterThan(theta - thetao, dvec3(M_PI))) * (theta - dvec3(2.0 * M_PI)) - thetao) / dt;
	omega = clamp(omega, max(-Model.omegamax, (-Model.thetamax - thetao) / dt), min(Model.omegamax, (Model.thetamax - thetao) / dt));
	theta = ang(thetao + omega * dt);
	//alpha = (omega - omegao) / dt;
}
#endif
void ctl::handle(dvec3 position, dvec3 attitude, dvec3 linearvelocity, dvec3 angularvelocity)
{
	x = position;
	theta = attitude;
	v = linearvelocity;
	omega = angularvelocity;
	if(Model.xsigma)
		Filter[0].handle(x, x);
	if(Model.thetasigma)
		Filter[1].handle(theta, theta);
}
