#ifndef _OUTPUT_HPP_
#define _OUTPUT_HPP_

#include "default.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#if(VIDEO)
#include "opengl.hpp"
#endif
#include "glm/glm.hpp"
#if(!MODEL)
#include "ros.hpp"
using namespace ros;
#endif
using namespace std;
using namespace glm;

class ntf
{
	public:
		ntf(void){};
		~ntf(void){};
		static void start(void);
		static void finish(void);
		static void cli(string);
		static void file(string, string);
		#if(VIDEO)
		static void gui(string, ivec2);
		#endif
		#if(!MODEL)
		static void socket0(int);
		static void socket1(dvec4);
		#endif
};

#include "info.hpp"

#endif
