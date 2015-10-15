
#ifndef RC_PARAMETRIC_SHAPES_H
#define RC_PARAMETRIC_SHAPES_H

#include "RenderChimp.h"

/*
	Circle ring

	@param res_radius
	tessellation resolution (nbr of vertices) in the radial direction ( inner_radius < radius < outer_radius )

	@param res_theta
	tessellation resolution (nbr of vertices) in the angular direction ( 0 < theta < 2PI )

	@param inner_radius
	radius of the innermost border of the ring

	@param outer_radius
	radius of the outermost border of the ring

	@return
	pointer to the generated VertexArray object

	CJG aug 2010 - created
*/
VertexArray* createCircleRing(const i32 &radius_res, const i32 &theta_res, const f32 &inner_radius, const f32 &outer_radius);

/*
	Sphere

	Author(s):
*/
VertexArray* createSphere(const i32 &res_theta, const i32 &res_phi, const f32 &radius);


/*
	Torus

	Author(s):
*/
VertexArray* createTorus(const i32 &res_theta, const i32 &res_phi, const f32 &rA, const f32 &rB);

#endif /* RC_PARAMETRIC_SHAPES_H */

