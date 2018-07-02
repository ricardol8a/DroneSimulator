#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include "default.hpp"
#if(VIDEO)
#include "config.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include "opengl.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/component_wise.hpp"
#include "geometry.hpp"
#include "config.hpp"
using namespace std;
using namespace glm;

class scn
{
	public:
		scn(void){};
		scn(cfg, cfg, cfg);
		~scn(void){};
		void shift(void);
		void move(bool, char);
		void update(void);
		void build(void);
		void render(void);
	private:
		unsigned int type;		//Type of Coordinates
		unsigned int view;		//Main View
		dvec3 p;			//Position Main Camera
		double near;
		double far;
		double angle;
		double ratio;
		double dt;
		double dr;
};

#include "space.hpp"
#include "plane.hpp"
#include "sector.hpp"
#include "camera.hpp"
#include "window.hpp"
#include "info.hpp"
#include "trajectory.hpp"
#include "robot.hpp"
#include "obstacle.hpp"
#include "register.hpp"
#endif

#endif
