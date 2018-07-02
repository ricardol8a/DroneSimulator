#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "default.hpp"
#if(VIDEO)
#include <cmath>
#include "opengl.hpp"
#include "glm/glm.hpp"
using namespace glm;

class scn;

class cam
{
	public:
		cam(void){};
		cam(dvec3, dvec3, dvec3, dvec3);
		cam(dvec3, dvec3, dvec3, double, double, double, double);
		~cam(void){};
		void update(dvec3, dvec3, dvec3, dvec3);
		void update(dvec3, dvec3, dvec3, double, double, double, double);
		void render(void);
	private:
		dvec3 x;		//Position in Cartesian Coordinates
		dvec3 u;		//Unit Vector of Direction
		dvec3 c;		//Center Box
		dvec3 d;		//Dimensions Box
		bool b;			//Projection
		friend class scn;
};

#endif
#endif
