//Headers
#include "graphics.hpp"

extern win Window;
#if(VIDEO)
extern scn Scene;
#endif

win Window;
#if(VIDEO)
scn Scene;
#endif

void grh::start(void)
{
	#if(VIDEO)
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	#else
	glutInitDisplayMode(GLUT_SINGLE);
	#endif
	cfg setwindow(string(CONFIG) + string("window.cfg"));
	Window = win(setwindow);
	#if(VIDEO)
	cfg setworld(string(CONFIG) + string("world.cfg"));
	cfg setscene(string(CONFIG) + string("scene.cfg"));
	Scene = scn(setscene, setwindow, setworld);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	#endif
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	atexit(finish);
}

void grh::finish(void)
{
}

void grh::display(void)
{
	#if(VIDEO)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Scene.update();
	Scene.render();
	glFlush();
	glutSwapBuffers();
	#else
	glClear(GL_COLOR_BUFFER_BIT);
	#endif
}

void grh::reshape(int width, int height)
{
	Window.update(ivec2(width, height));
	glutPostRedisplay();
}
