#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include "default.hpp"
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include "glm/glm.hpp"
#include "fl/FuzzyLogic.h"
#include "config.hpp"
using namespace std;
using namespace glm;
using namespace fl;

class ctl;

class ctr
{
	public:
		ctr(void){};
		ctr(cfg);
		~ctr(void){};
		void update(double, double, double, double);
        	void update(dvec3, dvec3, dvec3, dvec3);
		dvec3 calcule(dvec3, dvec3, dvec3);
	private:
		unsigned int type;
		dvec3 kp;
		dvec3 ki;
		dvec3 kd;
		dvec3 ko;
		CFuzzyEngine* FuzzyEngine;
		friend class ctl;	
};

#endif
