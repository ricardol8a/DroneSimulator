//Headers
#include "clock.hpp"

clk::clk(cfg config)
{
	to = config.getValueDouble("starttime");
	ts = config.getValueDouble("steptime");
	tf = config.getValueDouble("stoptime");
	t = to;
}

bool clk::update(void)
{
	if(t < tf)
	{
		//t = (static_cast<double>(t.size()) * ts + to);
		t = t + ts;
		t = round(t / ts) * ts;			//Correction
		return true;
	}
	else
		return false;
}
