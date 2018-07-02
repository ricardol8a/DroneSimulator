//Headers
#include "obstacle.hpp"

obs::obs(cfg config)
{
	x = dvec3(config.getValueVectorDouble("position", 0), config.getValueVectorDouble("position", 1), config.getValueVectorDouble("position", 2));
	d = dvec3(config.getValueVectorDouble("size", 0), config.getValueVectorDouble("size", 1), config.getValueVectorDouble("size", 2));
}

#if (VIDEO)
void obs::build(void)
{
	clr Color;
	#if (VIDEO > 2)
	glEnable(GL_LIGHT0);
	#endif
	#if (VIDEO > 1)
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	#endif
	list = glGenLists(1);
	glNewList(list, GL_COMPILE);
		#if (VIDEO > 2)
		glEnable(GL_LIGHTING);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Color.yellow);
		#else
		glColor4fv(Color.yellow);
		#endif
		#if (VIDEO > 1)
		glutSolidCube(1.0);
		#else
		glutWireCube(1.0);
		#endif
		#if (VIDEO > 2)
		glDisable(GL_LIGHTING);
		#endif
	glEndList();
}

void obs::render(void)
{
	glPushMatrix();
		glTranslatef(x.x, x.y, x.z);
		glScalef(d.x, d.y, d.z);
		glCallList(list);
	glPopMatrix();
}
#endif
