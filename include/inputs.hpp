#ifndef _INPUTS_HPP_
#define _INPUTS_HPP_

#include "default.hpp"
#include <string>
#include <cstdlib>
#include "opengl.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "config.hpp"
#if(!MODEL)
#include "ros.hpp"
using namespace ros;
#endif
using namespace glm;

class hid
{
	public:
		hid(void){};
		~hid(void){};
		static void start(void);
		static void finish(void);
		static void mouse(int, int, int, int);
		static void keyboard(unsigned char, int, int);
		static void extrakeyboard(int, int, int);
		static void joystick(unsigned int, int, int, int);
		#if(!MODEL)
		static void socket0(const ardrone_autonomy::Navdata&);
		static void socket1(const tf2_msgs::TFMessageConstPtr&);
		#endif
};

#if(VIDEO)
#include "scene.hpp"
#endif
#include "system.hpp"
#if(!MODEL)
#include "link.hpp"
#endif

#endif
