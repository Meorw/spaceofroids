
#ifndef RC_INTERPOLATION_H
#define RC_INTERPOLATION_H

#include "RenderChimp.h"

/*
	Linear interpolation
		
	Author(s):
*/
vec3f evalLERP(vec3f &p0, vec3f &p1, const f32 x);

/*
	Catmull-Rom spline interpolation

	Author(s):
*/
vec3f evalCatmullRom(vec3f &p0, vec3f &p1, vec3f &p2, vec3f &p3, const f32 t, const f32 x);

#endif /* RC_INTERPOLATION_H */

