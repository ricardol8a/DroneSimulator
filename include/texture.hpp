#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include "default.hpp"
#if (VIDEO)
#include "config.hpp"
#include <string>
#include <cmath>
#include "opengl.hpp"
#include "color.hpp"

struct tex
{
	GLuint id;
	unsigned int size;
	tex(unsigned int n)
	{
		clr Color;
		#if(VIDEO > 2)
		cfg setwindow(string(CONFIG) + string("window.cfg"));
		size = static_cast<unsigned int>(fmin(setwindow.getValueVectorInt("size", 0), setwindow.getValueVectorInt("size", 1)));
		#else
		size = 128;
		#endif
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		#if (VIDEO > 2)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		#else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		#endif
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		#if (VIDEO > 2)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2);
		#endif
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		#if (VIDEO > 2)
	    	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	    	#endif
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glViewport(0, 0, size, size);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, size, 0, size);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glBegin(GL_LINES);
            unsigned int p = 1;
            #ifdef __APPLE__
                p--;
            #endif
			for (unsigned int j = 0; j < 2; j++)
			{
				for (unsigned int i = 0; i < n + 1; i += (!j + n * j))
				{
					if (j)
						glColor4fv(Color.black);
					else
						glColor4fv(Color.silver);
					glVertex2i((size - 1) * i / n + p, p);
					glVertex2i((size - 1) * i / n + p, (size - 1) + p);
					glVertex2i(p, (size - 1) * i / n + p);
					glVertex2i((size - 1) + p, (size - 1) * i / n + p);
				}
			}
		glEnd();
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, size, size, 0);
	}
};
#endif

#endif
