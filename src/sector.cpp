//Headers
#include "sector.hpp"

#if (VIDEO)
box::box(ivec2 position, ivec2 dimension, ivec2 totaldimension)
{	
	x = position;
	d = dimension;
	rx = dvec2(position) / dvec2(totaldimension);
	rd = dvec2(dimension) / dvec2(totaldimension);
}

void box::update(ivec2 totaldimension)
{
	x = ivec2(rx * dvec2(totaldimension));
	d = ivec2(rd * dvec2(totaldimension));
}

void box::render(void)
{
	glViewport(x.x, x.y, d.x, d.y);
}
#endif

