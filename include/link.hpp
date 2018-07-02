#ifndef _LINK_HPP_
#define _LINK_HPP_

#include "default.hpp"
#if(!MODEL)
#include <vector>
#include <algorithm>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "geometry.hpp"
#include "config.hpp"

using namespace std;
using namespace glm;

class lnk
{
	public:
		lnk(void){};
		lnk(cfg);
		~lnk(void){};
		void add(int);
		void add(dvec3, dquat, bool, int);
		void sync(void);
	private:
		unsigned int type;
		int status;
		vector<vector<dmat4>> data;
		double alpha;
};

#include "system.hpp"

#endif
#endif
