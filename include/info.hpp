#ifndef _INFO_HPP_
#define _INFO_HPP_

#include "default.hpp"
#include <ctime>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#if(VIDEO)
#include "opengl.hpp"
#include "color.hpp"
#endif
#include "glm/glm.hpp"
#include "config.hpp"
using namespace std;
using namespace glm;

class txt
{
	public:
		txt(void){};
		txt(cfg);
		~txt(void){};
		void print(void);
		void store(void);
		#if(VIDEO)
		void render(void);
		#endif
	private:
		bool a;
		bool b;
};

#if(VIDEO)
#include "sector.hpp"
#include "plane.hpp"
#endif
#include "output.hpp"
#include "system.hpp"
#include "register.hpp"

#endif
