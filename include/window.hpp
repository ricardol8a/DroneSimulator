#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include "default.hpp"
#include <string>
#include "opengl.hpp"
#include "glm/glm.hpp"
#include "config.hpp"
using namespace std;
using namespace glm;
class scn;

class win
{
	public:
		win(void){};
		win(cfg);
		~win(void){};
		void update(ivec2);
	private:
		ivec2 x;		//Position
		ivec2 d, dmin, dmax;	//Dimensions
		string tt;		//Title
		friend class scn;
};

#endif
