#ifndef _TRAYECTORY_HPP_
#define _TRAYECTORY_HPP_

#include "default.hpp"
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#if(VIDEO)
#include "opengl.hpp"
#include "color.hpp"
#endif
#include "glm/glm.hpp"
#include "geometry.hpp"
#include "curves.hpp"
#include "config.hpp"
using namespace std;
using namespace glm;
#if(VIDEO)
class spc;
#endif
class reg;
class sys;

class pth
{
	public:
		pth(void){};
		pth(cfg);
		~pth(void){};
		dvec3 function(void);
		void update(void);
		#if (VIDEO)
		void build(void);
		void render(void);
		#endif
	private:
		unsigned int type;		//Type of Curve
		dvec3 x;		//Point
		dvec3 theta;
		dvec3 v;
		dvec3 omega;
		dvec3 a;
		dvec3 alpha;
		dvec3 c;		//Center
		dvec3 d;		//Coefficients
		dvec3 n;		//Normal Vector
		double angle;		//Angle
		dmat3 R;		//Matrix Rotation
		double b;		//Bias
		double k;		//Turns
		double l;		//Sections
		bool f;			//Compass
		double g;		//Gap
		#if (VIDEO)
		GLuint list;
		friend class spc;
		#endif
		friend class reg;
		friend class sys;
};

#include "clock.hpp"

#endif
