#ifndef _FILTER_HPP_
#define _FILTER_HPP_

#include "default.hpp"
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include "glm/glm.hpp"
#include "config.hpp"
using namespace std;
using namespace glm;

class ctl;

class ftr
{
	public:
		ftr(void){};
		ftr(cfg);
		~ftr(void){};
		void update(vector<dvec3>, vector<dvec3>);
		void update(vector<double>, vector<double>);
        	void update(dvec3, dvec3);
        	void handle(vector<dvec3>, vector<dvec3>);
        	void handle(dvec3, dvec3);
		dvec3 calcule(dvec3, dvec3);
	private:
		unsigned int type;
		vector<dvec3> a;
		vector<dvec3> b;
		vector<dvec3> in;
		vector<dvec3> out;
		friend class ctl;	
};

#endif
