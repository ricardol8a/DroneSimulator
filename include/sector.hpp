#ifndef _SECTOR_HPP_
#define _SECTOR_HPP_

#include "default.hpp"
#if (VIDEO)
#include "opengl.hpp"
#include "glm/glm.hpp"
using namespace glm;

class pln;
class scn;
class txt;

class box
{
	public:
		box(void){};
		box(ivec2, ivec2, ivec2);
		~box(void){};
		void update(ivec2);
		void render(void);
	private:
		ivec2 x;		//Position
		ivec2 d;		//Dimensions
		dvec2 rx;		//Relation of Position
		dvec2 rd;		//Relation of Dimensions
		friend class pln;
		friend class scn;
		friend class txt;
};
#endif

#endif
