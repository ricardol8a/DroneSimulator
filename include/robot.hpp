#ifndef _ROBOT_HPP_
#define _ROBOT_HPP_

#include "default.hpp"
#include <cmath>
#include <vector>
#include <functional>
#if (VIDEO)
#include "opengl.hpp"
#include "color.hpp"
#endif
#include "glm/glm.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtx/component_wise.hpp"
#include "geometry.hpp"
#include "calculus.hpp"
#include "config.hpp"
using namespace std;
using namespace glm;
class reg;
class scn;
class sys;

class rbt
{
	public:
		rbt(void);
		rbt(cfg);
		~rbt(void){};
		#if(MODEL)
		void update(dvec3, dvec3, dvec3, dvec3, dvec3, dvec3, dvec4);
		#else
		void update(dvec3, dvec3);
		#endif
		void handle(dvec3, dvec3);
		#if (VIDEO)
		void build(void);
		void render(void);
		#endif
	private:
		dvec3 x;		//Position
		dvec3 theta;		//Attitude
		dvec3 v;		//Linear Velocity
		dvec3 omega;		//Angular Velocity
		dvec3 vb;		//Linear Velocity Body
		dvec3 omegab;		//Angular Velocity Body
		dvec3 a;		//Linear Accelation
		dvec3 alpha;		//Angular Accelation
		dvec3 fb;		//Force Body
		dvec3 taob;		//Torque Body
		dvec4 w;		//Motors Speed
		dmat3 R;		//Transformation Matrix Linear
		dmat3 W;		//Transformation Matrix Angular
		dvec3 d;
		double r;
		dvec3 n;
		dvec3 eta;
		#if (VIDEO)
		GLuint list;
		#endif
		friend class reg;
		friend class scn;
		friend class sys;
};

#include "clock.hpp"
#include "model.hpp"

#endif
