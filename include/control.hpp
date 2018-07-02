#ifndef _CONTROL_HPP_
#define _CONTROL_HPP_

#include "default.hpp"
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include "glm/glm.hpp"
#include "geometry.hpp"
#include "controller.hpp"
#include "filter.hpp"
#include "config.hpp"
using namespace std;
using namespace glm;
class sys;

class ctl
{
	public:
		ctl(void){};
		ctl(cfg);
		~ctl(void){};
		#if(CONTROL)
		void update(dvec3, dvec3, dvec3, dvec3, dvec3, dvec3, dvec3, dvec3, dvec3, dvec3, dvec3, dvec3);
		#else
		void update(dvec3, dvec3);
		#endif
		void handle(dvec3, dvec3, dvec3, dvec3);
	private:
		dvec3 x;		//Position
		dvec3 theta;		//Attitude
		dvec3 v;		//Linear Velocity
		dvec3 omega;		//Angular Velocity
		dvec3 a;		//Linear Accelation
		dvec3 alpha;		//Angular Accelation
		dvec3 fb;		//Force Body
		dvec3 taob;		//Torque Body
		dvec4 w;		//Motors Speed
		dvec3 e;
		dvec3 ie;
		dvec3 de;
		dvec3 epsilon;
		dvec3 iepsilon;
		dvec3 depsilon;
        	dvec3 esigma;
        	dvec3 epsilonsigma;
		vector<ctr> Controller;
        	vector<ftr> Filter;
		friend class sys;
};

#include "clock.hpp"
#include "model.hpp"

#endif
