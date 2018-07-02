//Headers
#include "plane.hpp"

#if (VIDEO)
extern vector<box> Sector;

pln::pln(ivec2 position, ivec2 dimension)
{
	x = position;
	d = dimension;
}

void pln::update(ivec2 dimension)
{
	d = dimension;
}

void pln::build(void)
{
	clr Color;
	list = glGenLists(1);
	glNewList(list, GL_COMPILE);
		ivec2 p, q;
		glColor4fv(Color.black);
		for (unsigned int i = 0; i < Sector.size(); i++)
		{
			p = Sector[i].x + x - d / 2;
			#ifdef __APPLE__
			p--;
			#endif
			q = p + Sector[i].d;
			glBegin(GL_LINE_LOOP);
				glVertex2i(p.x + 1, p.y + 1);
				glVertex2i(q.x, p.y + 1);
				glVertex2i(q.x, q.y);
				glVertex2i(p.x + 1, q.y);
			glEnd();
		}
	glEndList();
}

void pln::render(void)
{
	glPushMatrix();
		glCallList(list);
	glPopMatrix();
}
#endif
