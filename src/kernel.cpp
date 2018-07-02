//Headers
#include "kernel.hpp"

extern sys System;
extern txt Info;
#if(VIDEO)
extern scn Scene;
#endif
#if(!MODEL)
extern lnk Link;
#endif

tmr Chrono;
sys System;

void knl::start(void)
{
	cfg setsystem(string(CONFIG) + string("system.cfg"));
	System = sys(setsystem);
	Info.print();
	#if(VIDEO)
	Scene.build();
	#endif
	cfg settime(string(CONFIG) + string("time.cfg"));
	Chrono = tmr(settime);
	glutIdleFunc(engine);
	atexit(finish);
	glutMainLoop();
}

void knl::finish(void)
{
	Info.store();
	Chrono.stadistics();
}

void knl::engine(void)
{
	#if(!MODEL)
	if(ok())
	{
		Link.sync();
		#endif
		System.automatic();
		Chrono.timing();
		Info.print();
		#if(VIDEO)
		glutPostRedisplay();
		#endif
		#if(!MODEL)
		spinOnce();
	}
	#endif
}
