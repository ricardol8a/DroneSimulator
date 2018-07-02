//Headers
#include "base.hpp"

void bas::start(void)
{
	srand(time(NULL));
	int i = 0;
	glutInit(&i, nullptr);
	#if(!MODEL)
	ros::init(i, nullptr, "setup");
	ros::start();
	#endif
	atexit(finish);
}

void bas::finish(void)
{
	#if(!MODEL)
	ros::shutdown();
	#endif
}
