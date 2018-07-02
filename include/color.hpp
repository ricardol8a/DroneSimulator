#ifndef _COLOR_HPP_
#define _COLOR_HPP_

#include "default.hpp"
#include "opengl.hpp"
#if(VIDEO)
struct clr
{
	GLfloat white[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat silver[4] = { 0.75, 0.75, 0.75, 1.0 };
	GLfloat gray[4] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat black[4] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat red[4] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat maroon[4] = { 0.5, 0.0, 0.0, 1.0 };
	GLfloat yellow[4] = { 1.0, 1.0, 0.0, 1.0 };
	GLfloat olive[4] = { 0.5, 0.5, 0.0, 1.0 };
	GLfloat lime[4] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat green[4] = { 0.0, 0.5, 0.0, 1.0 };
	GLfloat aqua[4] = { 0.0, 1.0, 1.0, 1.0 };
	GLfloat teal[4] = { 0.0, 0.5, 0.5, 1.0 };
	GLfloat blue[4] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat navy[4] = { 0.0, 0.0, 0.5, 1.0 };
	GLfloat fuchsia[4] = { 1.0, 0.0, 1.0, 1.0 };
	GLfloat purple[4] = { 0.5, 0.0, 0.5, 1.0 };
};
#endif

#endif
