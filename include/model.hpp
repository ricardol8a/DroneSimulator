#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include "default.hpp"
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include "glm/glm.hpp"
#include "glm/gtc/random.hpp"
#include "config.hpp"
using namespace std;
using namespace glm;
class rbt;
class ctl;
class txt;
class sys;

class mdl
{
	public:
		mdl(void){};
		mdl(cfg);
		~mdl(void){};
		void update(void);
	private:
		unsigned int level;
		unsigned int type;
		dvec3 ws;
		dvec3 wsmiu;
		double wssigma;
		double g;
		double m;		//Mass
		double k;
		double l;
		dmat3 I;
		double kl;
		double kd;
		double kf;
		double ir;
		double rm;
		double km;
		double taom;
		dmat4 M;
		double xsigma;
		double thetasigma;
		dvec3 xmax;		//Maximum Distance
		dvec3 thetamax;		//Maximum Inclination
		dvec3 vmax;		//Maximum Linear Velocity
		dvec3 omegamax;		//Maximum Angular Velocity
		double wmin;		//Minimum Motor Speed
		double wmax;		//Maximum Motor Speed
		double w2min;		//Square Minimum Motor Speed
		double w2max;		//Square Maximum Motor Speed
		friend class rbt;
		friend class ctl;
		friend class txt;
		friend class sys;
};

#endif
