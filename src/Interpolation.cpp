

#include "RenderChimp.h"

/*
	Linear interpolation
		
	Author(s):
*/
vec3f evalLERP(vec3f &p0, vec3f &p1, const f32 x)
{
	/* Implement me!*/
	return p0 + (p1 - p0) * x; // p0 - x * p0 + x * p1
}

/*
	Catmull-Rom spline interpolation

	Author(s):
*/
vec3f evalCatmullRom(vec3f &p0, vec3f &p1, vec3f &p2, vec3f &p3, const f32 t, const f32 x)
{
	/*vec3f c0, c1, c2, c3;
	c0 = p1;
    c1 = 0.5f*(p2-p0);
    c2 = -3.0f*p1 + 3.0f*p2 - 2*t*(p2-p0) - t*(p3-p1);
    c3 = 2.0f*p1 - 2.0f*p2 + t*(p2-p0) + t*(p3-p1);
	float x2 = x*x;
    float x3 = x2 * x;
    return c0 + c1*x + c2*x2 + c3*x3;*/

	float x2 = x*x,
		  x3 = x2*x;
	return 
		p0 * (-t*x + 2*t*x2 - t*x3) +
		p1 * (1 + x2*t - 3*x2 + x3*2 - x3*t) +
		p2 * (x*t + x2*3 - 2*t*x2 + x3*t - 2*x3) +
		p3 * (x3*t - x2*t);
}