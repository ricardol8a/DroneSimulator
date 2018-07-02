//Headers
#include "window.hpp"

win::win(cfg config)
{	
	tt = config.getValueString("title");
	#if(VIDEO)
	dmax = ivec2(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
	dmin = ivec2(640, 480);
	d = clamp(ivec2(config.getValueVectorInt("size", 0), config.getValueVectorInt("size", 1)), dmin, dmax);
	x = (dmax - d) / 2;
	#else
	dmax = ivec2(glutGet(GLUT_SCREEN_WIDTH), 1);
	dmin = ivec2(glutGet(GLUT_SCREEN_WIDTH), 1);
	d = (dmax + dmin) / 2;
	x = ivec2();	
	#endif
	glutInitWindowSize(d.x, d.y);
	glutInitWindowPosition(x.x, x.y);
	glutCreateWindow(tt.data());
}

void win::update(ivec2 dimension)
{
	bool b = false;
	if (dimension.x < dmin.x)
	{
		d.x = dmin.x;
		b = true;
	}
	else
	{
		if(dimension.x > dmax.x)
		{
			d.x = dmax.x;
			b = true;
		}
		else
			d.x = dimension.x;
	}
	if (dimension.y < dmin.y)
	{
		d.y = dmin.y;
		b = true;
	}
	else
	{
		if(dimension.y > dmax.y)
		{
			d.y = dmax.y;
			b = true;
		}
		else
			d.y = dimension.y;
	}
	if(b)
		glutReshapeWindow(d.x, d.y);
}

//GLUT_FULL_SCREEN, GLUT_WINDOW_X, GLUT_WINDOW_Y, glutLeaveFullScreen, glutFullScreenToggle, glutSetWindowTitle, glutShowWindow, glutHideWindow, glutReshapeWindow, glutPositionWindow

