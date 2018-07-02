#ifndef _GRAPHICS_HPP_
#define _GRAPHICS_HPP_

#include "default.hpp"
#include <string>
#include <cstdlib>
#include "opengl.hpp"
#include "glm/glm.hpp"
#include "config.hpp"

class grh
{
	public:
		grh(void){};
		~grh(void){};
		static void start(void);
		static void finish(void);
		static void display(void);
		static void reshape(int, int);
};

#include "window.hpp"
#if(VIDEO)
#include "scene.hpp"
#endif

#endif
