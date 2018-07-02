#ifndef _SYSTEM_HPP_
#define _SYSTEM_HPP_

#include "default.hpp"
#include <cmath>
#include <vector>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "config.hpp"
#if(!MODEL)
#include "geometry.hpp"
#include "ros.hpp"
using namespace ros;
#endif
using namespace std;
using namespace glm;

class txt;

class sys
{
	public:
		sys(void){};
		sys(cfg);
		~sys(void){};
		void admin(void);
		void manual(bool, bool, char);
		void automatic(void);
		void focus(int);
		#if(!MODEL)
		void assign(bool, dvec3, dquat);
		#endif
	private:
		double dt;		//Default Translation
		double dr;		//Default Rotation
		bool state;
		unsigned int n;
		#if(!MODEL)
		unsigned int k;
		unsigned int kmax;
		#endif
		friend class txt;
};

#include "clock.hpp"
#include "trajectory.hpp"
#include "control.hpp"
#include "robot.hpp"
#include "obstacle.hpp"
#include "register.hpp"
#include "model.hpp"
#if(!MODEL)
#include "output.hpp"
#include "link.hpp"
#endif

#endif
