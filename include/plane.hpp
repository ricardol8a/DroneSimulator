#ifndef _PLANE_HPP_
#define _PLANE_HPP_

#include "default.hpp"
#if (VIDEO)
#include <vector>
#include "opengl.hpp"
#include "color.hpp"
#include "glm/glm.hpp"
using namespace std;
using namespace glm;

class scn;
class txt;

class pln
{
	public:
		pln(void){};
		pln(ivec2, ivec2);
		~pln(void){};
		void update(ivec2);
		void build(void);
		void render(void);
	private:
		ivec2 x;		//Position
		ivec2 d;		//Dimensions
		GLuint list;
		friend class scn;
		friend class txt;
};

#include "sector.hpp"
#endif

#endif
