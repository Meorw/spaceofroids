/*
 */

#ifndef RC_PLANE_H
#define RC_PLANE_H

class Plane {

	public:

		Plane() {}
		~Plane() {}

		vec3f project(
				vec3f			*point
			);

	public:
		
		f32		a;
		f32		b;
		f32		c;
		f32		d;

};

#endif /* RC_PLANE_H */

