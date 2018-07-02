#ifndef _SPACE_HPP_
#define _SPACE_HPP_

#include "default.hpp"
#if (VIDEO)
#include <iostream>
#include <cmath>
#include "opengl.hpp"
#include "color.hpp"
#include "texture.hpp"
#include "glm/glm.hpp"
using namespace std;
using namespace glm;

class scn;

class spc
{
	public:
		spc(void){};
		spc(dvec3, double, double);
		~spc(void){};
		void update(double);
		void build(void);
		void render(void);
	private:
		dvec3 x;		//Position
		double d;		//Dimensions
		double dmin;		//Min Dimensions
		double dmax;		//Max Dimensions
		GLuint list;
		friend class scn;
};

#endif

#endif
