#ifndef _CLOCK_HPP_
#define _CLOCK_HPP_

#include "default.hpp"
#include <cmath>
#include "config.hpp"
class env;
class pth;
class rbt;
class reg;
class sys;
class ctr;
class ctl;

class clk
{
	public:
		clk(void){};
		clk(cfg);
		~clk(void){};
		bool update(void);
	private:
		double t;
		double to;
		double ts;
		double tf;
		friend class env;
		friend class pth;
		friend class rbt;
		friend class reg;
		friend class sys;
		friend class ctr;
		friend class ctl;
};

#endif
