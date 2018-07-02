#ifndef _BASE_HPP_
#define _BASE_HPP_

#include "default.hpp"
#include <cstdlib>
#include <ctime>
#include "opengl.hpp"
#if(!MODEL)
#include "ros.hpp"
using namespace ros;
#endif

class bas
{
	public:
		bas(void){};
		~bas(void){};
		static void start(void);
		static void finish(void);
};

#endif
