//Header
#include "robot.hpp"

extern clk Clock;
extern mdl Model;

rbt::rbt(cfg config)
{
	x = dvec3(config.getValueVectorDouble("position", 0), config.getValueVectorDouble("position", 1), config.getValueVectorDouble("position", 2));
	theta = ang(dvec3(config.getValueVectorDouble("attitude", 0), config.getValueVectorDouble("attitude", 1), config.getValueVectorDouble("attitude", 2)) * (M_PI / 180.0));
	v = dvec3(config.getValueVectorDouble("linearvelocity", 0), config.getValueVectorDouble("linearvelocity", 1), config.getValueVectorDouble("linearvelocity", 2));
	omega = dvec3(config.getValueVectorDouble("angularvelocity", 0), config.getValueVectorDouble("angularvelocity", 1), config.getValueVectorDouble("angularvelocity", 2)) * (M_PI / 180.0);
	R = dmat3(cos(theta.z) * cos(theta.y), sin(theta.z) * cos(theta.y), -sin(theta.y), cos(theta.z) * sin(theta.y) * sin(theta.x) - sin(theta.z) * cos(theta.x), sin(theta.z) * sin(theta.y) * sin(theta.x) + cos(theta.z) * cos(theta.x), cos(theta.y) * sin(theta.x), cos(theta.z) * sin(theta.y) * cos(theta.x) + sin(theta.z) * sin(theta.x), sin(theta.z) * sin(theta.y) * cos(theta.x) - cos(theta.z) * sin(theta.x), cos(theta.y) * cos(theta.x));
	W = dmat3(1.0, 0.0, 0.0, 0.0, cos(theta.x), -sin(theta.x), -sin(theta.y), cos(theta.y) * sin(theta.x), cos(theta.y) * cos(theta.x));
	vb = inverse(R) * v;
	omegab = W * omega;
	a = dvec3(config.getValueVectorDouble("linearacceleration", 0), config.getValueVectorDouble("linearacceleration", 1), config.getValueVectorDouble("linearacceleration", 2));
	alpha = dvec3(config.getValueVectorDouble("angularacceleration", 0), config.getValueVectorDouble("angularacceleration", 1), config.getValueVectorDouble("angularacceleration", 2)) * (M_PI / 180.0);
	if(config.getIfExistSymbol("thrust") && config.getIfExistSymbol("torque"))
	{
		fb = dvec3(0.0, 0.0, config.getValueDouble("thrust"));
		taob = dvec3(config.getValueVectorDouble("torque", 0), config.getValueVectorDouble("torque", 1), config.getValueVectorDouble("torque", 2));
	}
	else
	{
		fb = Model.m * dvec3(0.0, 0.0, Model.g);
		taob = dvec3();
	}
	if(config.getIfExistSymbol("speedmotor"))
		w = dvec4(config.getValueVectorDouble("speedmotor", 0), config.getValueVectorDouble("speedmotor", 1), config.getValueVectorDouble("speedmotor", 2), config.getValueVectorDouble("speedmotor", 3));
	else
		w = dvec4(sqrt(fb.z / Model.kl) / 2.0);
	d = dvec3(config.getValueVectorDouble("size", 0), config.getValueVectorDouble("size", 1), config.getValueVectorDouble("size", 2));
	r = length(d) / 2.0;
	if ((x != clamp(x, -Model.xmax, Model.xmax)) || (theta != clamp(theta - (2.0 * M_PI) * roundEven(theta / (2.0 * M_PI)), -Model.thetamax, Model.thetamax)) || (v != clamp(v, -Model.vmax, Model.vmax)) || (omega != clamp(omega, -Model.omegamax, Model.omegamax)) || (w != clamp(w, Model.wmin, Model.wmax)))
		throw "Error: Init Value Out of Bounds";
	n = dvec3();
	eta = dvec3();
}
#if(MODEL)
void rbt::update(dvec3 position, dvec3 attitude, dvec3 linearvelocity, dvec3 angularvelocity, dvec3 thrust, dvec3 torque, dvec4 speedmotor)
{
	double dt = Clock.ts;
	double t = Clock.t;
	double to = t - dt;
	if(Model.xsigma)
	{
		x = x - n;
		v = v - n / dt;
		a = a - n / pow(dt, 2.0);
	}
	if(Model.thetasigma)
	{
		theta = ang(theta - eta);
		omega = omega - eta / dt;
		alpha = alpha - eta / pow(dt, 2.0);
	}
	dvec3 xo = x;
	dvec3 thetao = theta;
	dvec3 vo = v;
	dvec3 omegao = omega;
	if(Model.level)
	{
		if(Model.level > 1)
		{
			if(Model.level > 2)
			{
				dvec4 wo = w;
				function<dvec4(double, dvec4)> funcw = [=](double, dvec4 var) -> dvec4
				{
					return (Model.km * speedmotor - var) / Model.taom;
				};
				w = Differentiation(funcw, wo, to, dt, t);
				dvec4 w2 = pow(w, dvec4(2.0));
				fb = dvec3(0.0, 0.0, Model.kl *(w2[3] + w2[2] + w2[1] + w2[0]));
				taob = dvec3(sqrt(2.0) / 2.0 * Model.l * Model.kl * (w2[3] + w2[2] - w2[1] - w2[0]), sqrt(2.0) / 2.0 * Model.l * Model.kl * (- w2[3] + w2[2] + w2[1] - w2[0]), Model.kd * (w2[3] - w2[2] + w2[1] - w2[0]));
			}
			else
			{
				fb = thrust;
				taob = torque;
			}
			#if (MODEL < 3)
			function<dvec3(double, dvec3 var)> funcomegab = [=](double, dvec3 var) -> dvec3
			{
				return inverse(Model.I) * (taob - cross(var, Model.I * var) - Model.ir * cross(var, dvec3(0.0, 0.0, 1.0)) * (-w[3] + w[2] - w[1] + w[0]));
			};
			dvec3 omegabo = omegab;
			omegab = Differentiation(funcomegab, omegabo, to, dt, t);
			#if (MODEL == 1)
			function<dvec3(double, dvec3 var)> functheta = [=](double, dvec3 var) -> dvec3
			{
				dmat3 W = dmat3(1.0, 0.0, 0.0, 0.0, cos(var.x), -sin(var.x), -sin(var.y), cos(var.y) * sin(var.x), cos(var.y) * cos(var.x));
				return inverse(W) * omegab;
			};
			theta = ang(Differentiation(functheta, thetao, to, dt, t));
			#else
			dmat3 S = dmat3(0.0, omegab.z, -omegab.y, -omegab.z, 0.0, omegab.x, omegab.y, -omegab.x, 0.0);
			function<dmat3(double, dmat3 var)> funcR = [=](double, dmat3 var) -> dmat3
			{
				return var * S;
			};
			dmat3 Ro = R;
			R = Differentiation(funcR, Ro, to, dt, t);
			theta = ang(dvec3(atan2(R[1][2], R[2][2]), -asin(R[0][2]), atan2(R[0][1], R[0][0])));
			#endif
			omega = (dvec3(lessThan(theta - thetao, dvec3(-M_PI))) * (theta + dvec3(2.0 * M_PI)) + dvec3(greaterThan(theta - thetao, dvec3(-M_PI))) * dvec3(lessThan(theta - thetao, dvec3(M_PI))) * theta + dvec3(greaterThan(theta - thetao, dvec3(M_PI))) * (theta - dvec3(2.0 * M_PI)) - thetao) / dt;
			#else
			function<dvec3(double, vector<dvec3> var)> functhetaomega = [=](double, vector<dvec3> var) -> dvec3
			{
				dmat3 W = dmat3(1.0, 0.0, 0.0, 0.0, cos(var[0].x), -sin(var[0].x), -sin(var[0].y), cos(var[0].y) * sin(var[0].x), cos(var[0].y) * cos(var[0].x));
				dmat3 J = transpose(W) * Model.I * W;
				dmat3 C = dmat3(0.0, 0.5 * Model.I[0][0] * var[1].z * cos(var[0].y), -Model.I[0][0] * var[1].y * cos(var[0].y), 0.5 * (Model.I[1][1] - Model.I[2][2]) * var[1].y * sin(2.0 * var[0].x) + 0.5 * (Model.I[2][2] - Model.I[1][1]) * var[1].z * cos(2.0 * var[0].x) * cos(var[0].y), (Model.I[2][2] - Model.I[1][1]) * var[1].x * sin(2.0 * var[0].x) + 0.25 * (Model.I[1][1] - Model.I[2][2]) * var[1].z * sin(2.0 * var[0].x) * sin(var[0].y), 0.5 * (Model.I[2][2] - Model.I[1][1]) * var[1].y * sin(2.0 * var[0].x) * sin(var[0].y) + (Model.I[1][1] - Model.I[2][2]) * var[1].x * cos(2.0 * var[0].x) * cos(var[0].y), 0.5 * (Model.I[2][2] - Model.I[1][1]) * var[1].z * sin(2.0 * var[0].x) * pow(cos(var[0].y), 2.0) + 0.5 * (Model.I[2][2] - Model.I[1][1]) * var[1].y * cos(2.0 * var[0].x) * cos(var[0].y) - Model.I[0][0] * var[1].y * cos(var[0].y), 0.25 * (Model.I[2][2] - Model.I[1][1]) * var[1].y * sin(2.0 * var[0].x) * sin(var[0].y) + 0.5 * Model.I[1][1] * var[1].z * pow(sin(var[0].x), 2.0) * sin(2.0 * var[0].y) + 0.5 * Model.I[2][2] * var[1].z * pow(cos(var[0].x), 2.0) * sin(2.0 * var[0].y) - 0.5 * Model.I[0][0] * var[1].z * sin(2.0 * var[0].y) + (Model.I[1][1] - Model.I[2][2]) * var[1].x * cos(2.0 * var[0].x) * cos(var[0].y) + 0.5 * Model.I[0][0] * var[1].x * cos(var[0].y), (Model.I[1][1] - Model.I[2][2]) * var[1].x * sin(2.0 * var[0].x) * pow(cos(var[0].y), 2.0) - Model.I[1][1] * var[1].y * pow(sin(var[0].x), 2.0) * sin(2.0 * var[0].y) - Model.I[2][2] * var[1].y * pow(cos(var[0].x), 2.0) * sin(2.0 * var[0].y) + Model.I[0][0] * var[1].y * sin(2.0 * var[0].y));
				return inverse(J) * (taob - C * var[1]);
			};
			vector<dvec3> thetaomega = {thetao, omegao};
			aux = Differentiation(functhetaomega, thetaomega, to, dt, t);
			theta = ang(thetaomega[0]);
			omega = thetaomega[1];
			#endif
			#if (MODEL != 2)
			R = dmat3(cos(theta.z) * cos(theta.y), sin(theta.z) * cos(theta.y), -sin(theta.y), cos(theta.z) * sin(theta.y) * sin(theta.x) - sin(theta.z) * cos(theta.x), sin(theta.z) * sin(theta.y) * sin(theta.x) + cos(theta.z) * cos(theta.x), cos(theta.y) * sin(theta.x), cos(theta.z) * sin(theta.y) * cos(theta.x) + sin(theta.z) * sin(theta.x), sin(theta.z) * sin(theta.y) * cos(theta.x) - cos(theta.z) * sin(theta.x), cos(theta.y) * cos(theta.x));
			#endif
			function<dvec3(double, dvec3 var)> funcv = [=](double, dvec3 var) -> dvec3
			{
				return (R * fb + 0.5 * Model.k * (Model.ws - var) * abs(Model.ws - var)) / Model.m - dvec3(0.0, 0.0, Model.g);
			};
			v = Differentiation(funcv, vo, to, dt, t);
		}
		else
		{
			v = linearvelocity;
			omega = angularvelocity;
			theta = ang(theta + omega * dt);
		}
		x = xo + v * dt;
	}
	else
	{
		x = position;
		theta = attitude;
	}
	v = (x - xo) / dt;
	omega = (dvec3(lessThan(theta - thetao, dvec3(-M_PI))) * (theta + dvec3(2.0 * M_PI)) + dvec3(greaterThan(theta - thetao, dvec3(-M_PI))) * dvec3(lessThan(theta - thetao, dvec3(M_PI))) * theta + dvec3(greaterThan(theta - thetao, dvec3(M_PI))) * (theta - dvec3(2.0 * M_PI)) - thetao) / dt;
	a = (v - vo) / dt;
	alpha = (omega - omegao) / dt;
	R = dmat3(cos(theta.z) * cos(theta.y), sin(theta.z) * cos(theta.y), -sin(theta.y), cos(theta.z) * sin(theta.y) * sin(theta.x) - sin(theta.z) * cos(theta.x), sin(theta.z) * sin(theta.y) * sin(theta.x) + cos(theta.z) * cos(theta.x), cos(theta.y) * sin(theta.x), cos(theta.z) * sin(theta.y) * cos(theta.x) + sin(theta.z) * sin(theta.x), sin(theta.z) * sin(theta.y) * cos(theta.x) - cos(theta.z) * sin(theta.x), cos(theta.y) * cos(theta.x));
	vb = inverse(R) * v;
	W = dmat3(1.0, 0.0, 0.0, 0.0, cos(theta.x), -sin(theta.x), -sin(theta.y), cos(theta.y) * sin(theta.x), cos(theta.y) * cos(theta.x));
	omegab = W * omega;
	if(Model.xsigma)
	{
		if(Model.type)
			n = gaussRand(dvec3(), dvec3(Model.xsigma));
		else
			n = linearRand(dvec3(-Model.xsigma), dvec3(Model.xsigma));
		x = x + n;
		v = v + n / dt;
		a = a + n / pow(dt, 2.0);
	}
	if(Model.thetasigma)
	{
		if(Model.type)
			eta = gaussRand(dvec3(), dvec3(Model.thetasigma));
		else
			eta = linearRand(dvec3(-Model.thetasigma), dvec3(Model.thetasigma));
		theta = ang(theta + eta);
		omega = omega + eta / dt;
		alpha = alpha + eta / pow(dt, 2.0);
	}
}
#else
void rbt::update(dvec3 position, dvec3 attitude)
{
	double dt = Clock.ts;
	dvec3 xo = x;
	dvec3 thetao = theta;
	dvec3 vo = v;
	dvec3 omegao = omega;
	x = position;
	theta = attitude;
	v = (x - xo) / dt;
	omega = (dvec3(lessThan(theta - thetao, dvec3(-M_PI))) * (theta + dvec3(2.0 * M_PI)) + dvec3(greaterThan(theta - thetao, dvec3(-M_PI))) * dvec3(lessThan(theta - thetao, dvec3(M_PI))) * theta + dvec3(greaterThan(theta - thetao, dvec3(M_PI))) * (theta - dvec3(2.0 * M_PI)) - thetao) / dt;
	a = (v - vo) / dt;
	alpha = (omega - omegao) / dt;
}
#endif

void rbt::handle(dvec3 position, dvec3 attitude)
{
	x = position;
	theta = attitude;
	#if(MODEL)
	if(Model.xsigma)
		x = x + n;
	if(Model.thetasigma)
		theta = ang(theta + eta);
	#endif
}

#if (VIDEO)
void rbt::build(void)
{
	clr Color;
	#if (VIDEO > 2)
	glEnable(GL_LIGHT0);
	#endif
	#if (VIDEO > 1)
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	#endif
	list = glGenLists(1);
	glNewList(list, GL_COMPILE);
		#if (VIDEO > 2)
		glEnable(GL_LIGHTING);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Color.black);
		#else
		glColor4fv(Color.black);
		#endif
		glPushMatrix();
			glTranslatef (d.x / 4.0 - d.z / 16.0, d.y / 4.0 - d.z / 16.0, 0.0);
			#if (VIDEO > 1)
			glutSolidTorus(d.z / 6.0, d.x / 4.0 - d.z / 12.0, 8, 20);
			#else
			glutWireTorus(d.z / 6.0, d.x / 4.0 - d.z / 12.0, 8, 20);
			#endif
		glPopMatrix();
		glPushMatrix();
			glTranslatef (-d.x / 4.0 + d.z / 16.0, d.y / 4.0 - d.z / 16.0, 0.0);
			#if (VIDEO > 1)
			glutSolidTorus(d.z / 6.0, d.x / 4.0 - d.z / 12.0, 8, 20);
			#else
			glutWireTorus(d.z / 6.0, d.x / 4.0 - d.z / 12.0, 8, 20);
			#endif
		glPopMatrix();
		glPushMatrix();
			glTranslatef (d.x / 4.0 - d.z / 16.0, -d.y / 4.0 + d.z / 16.0, 0.0);
			#if (VIDEO > 1)
			glutSolidTorus(d.z / 6.0, d.x / 4.0 - d.z / 12.0, 8, 20);
			#else
			glutWireTorus(d.z / 6.0, d.x / 4.0 - d.z / 12.0, 8, 20);
			#endif
		glPopMatrix();
		glPushMatrix();
			glTranslatef (-d.x / 4.0 + d.z / 16.0, -d.y / 4.0 + d.z / 16.0, 0.0);
			#if (VIDEO > 1)
			glutSolidTorus(d.z / 6.0, d.x / 4.0 - d.z / 12.0, 8, 20);
			#else
			glutWireTorus(d.z / 6.0, d.x / 4.0 - d.z / 12.0, 8, 20);
			#endif
		glPopMatrix();
		glPushMatrix();
			glScalef(d.x / 4.0, d.y / 8.0, d.z / 2.0);		
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glRotatef(90.0, 0.0, 1.0, 0.0);
			#if (VIDEO > 2)
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Color.red);
			#else
			glColor4fv(Color.red);
			#endif
			#if (VIDEO > 1)
			glutSolidSphere(1.0, 6, 5);
			#else
			glutWireSphere(1.0, 6, 5);
			#endif
		glPopMatrix();
		glPushMatrix();
			glTranslatef (d.x / 2.0, 0.0, 0.0);
			glRotatef(90.0, 0.0, 1.0, 0.0);
			#if (VIDEO > 2)
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Color.black);
			#else
			glColor4fv(Color.black);
			#endif
			#if (VIDEO > 1)
			glutSolidCone(d.z / 6.0, d.y / 8.0, 6, 1);
			#else
			glutWireCone(d.z / 6.0, d.y / 8.0, 6, 1);
			#endif
		glPopMatrix();
		#if (VIDEO > 2)
		glDisable(GL_LIGHTING);
		#endif
	glEndList();
}

void rbt::render(void)
{
	glPushMatrix();
		glTranslatef(x.x, x.y, x.z);
		glRotated(theta.z * (180.0 / M_PI), 0.0, 0.0, 1.0);
		glRotated(theta.y * (180.0 / M_PI), 0.0, 1.0, 0.0);
		glRotated(theta.x * (180.0 / M_PI), 1.0, 0.0, 0.0);
		glCallList(list);
	glPopMatrix();
}
#endif
