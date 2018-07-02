#ifndef _TIMER_HPP_
#define _TIMER_HPP_

#include "default.hpp"
#include <iostream>
#include <chrono>
#include "config.hpp"
using namespace std;
using namespace std::chrono;

class tmr
{
	public:
		tmr(void){};
		tmr(cfg);
		~tmr(void){};
		void timing(void);
		void stadistics(void);
	private:
		steady_clock::time_point to;
		steady_clock::time_point tf;
		steady_clock::time_point tp;
		steady_clock::duration drt;
		steady_clock::duration dft;
		steady_clock::duration dt;
		int total;
};

#endif
