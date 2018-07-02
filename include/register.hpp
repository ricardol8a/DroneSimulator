#ifndef _REGISTER_HPP_
#define _REGISTER_HPP_

#include "default.hpp"
#include <iostream>
#include <vector>
#if(VIDEO)
#include "opengl.hpp"
#include "color.hpp"
#endif
#include "glm/glm.hpp"
using namespace std;
using namespace glm;

class txt;

class reg
{
	public:
		reg(void){};
		reg(int);
		~reg(void){};
		void update(double, dvec3, dvec3, dvec3, dvec3, dvec3, dvec3, bool);
		#if(VIDEO)
		void render(void);
		#endif
	private:
		vector<double> data;
		vector<vector<double>> target;
		vector<vector<double>> status;
		friend class txt;
};

#endif

