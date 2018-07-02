//Headers
#include "timer.hpp"

tmr::tmr(cfg config)
{
	dft = steady_clock::duration::zero();
	drt = steady_clock::duration::zero();
	#if(MODEL)
	dt = chrono::microseconds(static_cast<int>(config.getValueDouble("steptime") * 1000000.0) * static_cast<int>(config.getValueBool("realtime")));
	#else
	dt = chrono::microseconds(static_cast<int>(config.getValueDouble("steptime") * 1000000.0));
	#endif
	total = 0;
	to = chrono::steady_clock::now();
}

void tmr::timing(void)
{
	if (total)
		to = tf;
	if (dt > steady_clock::duration::zero())
	{
		tp = chrono::steady_clock::now();
		if ((tp - to) < dt)
			while((chrono::steady_clock::now() - to) < dt);
			//this_thread::sleep_for(chrono::duration<double,micro>(dt - (tp - to)));
		else
			cerr<<"Warning: RunTime Exceeds StepTime"<<endl;
	}
	tf = chrono::steady_clock::now();
	if (dt > steady_clock::duration::zero())
		drt += tp - to;
	dft += tf - to;
	total++;
}

void tmr::stadistics(void)
{
	cout<<"Stadistics: RunTime = ";
	if (dt > steady_clock::duration::zero())
		cout<< chrono::duration<double, milli>(drt).count() / static_cast<double>(total) <<" ms | ";
	cout<<"FrameTime = "<< chrono::duration<double, milli>(dft).count() / static_cast<double>(total) <<" ms"<<endl;
}
