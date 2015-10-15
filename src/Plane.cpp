#include "RenderChimp.h"
#include "Plane.h"

vec3f Plane::project(
		vec3f			*point
	)
{
	f32 v = (a * point->x + b * point->y + c * point->z + d) /
		(a * a + b * b + c * c);

	return vec3f(
		point->x - a * v,
		point->y - b * v,
		point->z - c * v);
}
