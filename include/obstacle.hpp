#ifndef _OBSTACLE_HPP_
#define _OBSTACLE_HPP_

#include "default.hpp"
#if (VIDEO)
#include "opengl.hpp"
#include "color.hpp"
#endif
#include "glm/glm.hpp"
#include "config.hpp"
using namespace glm;
#if(VIDEO)
class spc;
#endif
class ctr;

class obs
{
	public:
		obs(void){};
		obs(cfg);
		~obs(void){};
		#if (VIDEO)
		void build(void);
		void render(void);
		#endif
	private:
		dvec3 x;		//Position
		dvec3 d;		//Dimensions
		#if (VIDEO)
		GLuint list;
		friend class spc;
		#endif
		friend class ctr;
};

#endif
