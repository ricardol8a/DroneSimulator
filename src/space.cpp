//Headers
#include "space.hpp"

#if (VIDEO)

spc::spc(dvec3 position, double mindimension, double maxdimension)
{
	x = position;
	dmin = mindimension;
	dmax = maxdimension;
	dmax = floor(dmax / dmin) * dmin;	//Correction
	d = dmin;
}

void spc::update(double dimension)
{
	d = dmin * ceil(dimension / dmin);
	if(d > dmax)
	{
		d = dmax;
		cerr << "Warning: Out of Bounds" << endl;
	}
}

void spc::build(void)
{
	clr Color;
	double l = d / 2.0;
	static unsigned int n = l;
	#if (VIDEO > 1)
	static tex Texture(n);
	list = glGenLists(1);
	glNewList(list, GL_COMPILE);
		glEnable(GL_TEXTURE_2D);
	    	glBindTexture(GL_TEXTURE_2D, Texture.id);
	    	glEnable(GL_CULL_FACE);
	    	glCullFace(GL_FRONT);
	    	glBegin(GL_QUADS);
		    	int t = static_cast<int>(l) / n;
			// Front Face
			glTexCoord2i(-t, -t);
			glVertex3f(-l, -l, l);
			glTexCoord2i(t, -t); 
			glVertex3f(l, -l, l);
			glTexCoord2i(t, t); 
			glVertex3f(l, l, l);
			glTexCoord2i(-t, t); 
			glVertex3f(-l, l, l);
			// Back Face
			glTexCoord2i(-t, -t); 
			glVertex3f(-l, -l, -l);
			glTexCoord2i(-t, t); 
			glVertex3f(-l, l, -l);
			glTexCoord2i(t, t); 
			glVertex3f(l, l, -l);
			glTexCoord2i(t, -t); 
			glVertex3f(l, -l, -l);
			// Top Face
			glTexCoord2i(-t, -t); 
			glVertex3f(-l, l, -l);
			glTexCoord2i(-t, t); 
			glVertex3f(-l, l, l);
			glTexCoord2i(t, t); 
			glVertex3f(l, l, l);
			glTexCoord2i(t, -t); 
			glVertex3f(l, l, -l);
			// Bottom Face
			glTexCoord2i(-t, -t); 
			glVertex3f(-l, -l, -l);
			glTexCoord2i(t, -t); 
			glVertex3f(l, -l, -l);
			glTexCoord2i(t, t); 
			glVertex3f(l, -l, l);
			glTexCoord2i(-t, t); 
			glVertex3f(-l, -l, l);
			// Right face
			glTexCoord2i(-t, -t); 
			glVertex3f(l, -l, -l);
			glTexCoord2i(t, -t); 
			glVertex3f(l, l, -l);
			glTexCoord2i(t, t); 
			glVertex3f(l, l, l);
			glTexCoord2i(-t, t); 
			glVertex3f(l, -l, l);
			// Left Face
			glTexCoord2i(-t, -t); 
			glVertex3f(-l, -l, -l);
			glTexCoord2i(-t, t); 
			glVertex3f(-l, -l, l);
			glTexCoord2i(t, t); 
			glVertex3f(-l, l, l);
			glTexCoord2i(t, -t); 
			glVertex3f(-l, l, -l);
	    	glEnd();
	    	glCullFace(GL_BACK);
	    	glDisable(GL_TEXTURE_2D);
	    	glBindTexture(GL_TEXTURE_2D, 0);
	glEndList();
	#else
	GLuint aux = glGenLists(1);
	glNewList(aux, GL_COMPILE);
		glBegin(GL_LINES);
			for (int j = 0; j < 2; j++)
			{
				for (int i = -l; i < l + 1; i += (!j + n * j))
				{
					if (j)
						glColor4fv(Color.black);
					else
						glColor4fv(Color.silver);
					glVertex2i(i, -l);
					glVertex2i(i, l);
					glVertex2i(-l, i);
					glVertex2i(l, i);	
				}
			}
		glEnd();
	glEndList();
	list = glGenLists(1);
	glNewList(list, GL_COMPILE);
		glPushMatrix();
			glTranslatef(l + x.x, x.y, x.z);
			glRotatef(90.0, 0.0, -1.0, 0.0);
			glCallList(aux);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-l + x.x, x.y, x.z);
			glRotatef(90.0, 0.0, -1.0, 0.0);
			glCallList(aux);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(x.x, l + x.y, x.z);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glCallList(aux);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(x.x, -l + x.y, x.z);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glCallList(aux);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(x.x, x.y, l + x.z);
			glCallList(aux);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(x.x, x.y, -l + x.z);
			glCallList(aux);
		glPopMatrix();
	glEndList(); 
	#endif
}

void spc::render(void)
{
	glPushMatrix();
		glCallList(list);
	glPopMatrix();
}
#endif
