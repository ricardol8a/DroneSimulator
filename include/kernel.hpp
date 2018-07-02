#ifndef _KERNEL_HPP_
#define _KERNEL_HPP_

#include "default.hpp"
#include <string>
#include <cstdlib>
#include "opengl.hpp"
#include "config.hpp"
#if(!MODEL)
#include "ros.hpp"
using namespace ros;
#endif
using namespace std;

class knl
{
	public:
		knl(void){};
		~knl(void){};
		static void start(void);
		static void finish(void);
		static void engine(void);
};

#include "system.hpp"
#if(VIDEO)
#include "scene.hpp"
#endif
#if(!MODEL)
#include "link.hpp"
#endif
#include "info.hpp"
#include "timer.hpp"

#endif
