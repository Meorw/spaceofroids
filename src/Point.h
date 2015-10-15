/*
 */

#ifndef __RC_POINT_H__
#define __RC_POINT_H__

class Point {

	bool inside(
			AABox	*aabox,
		);

	bool inside(
			Sphere	*sphere,
		);

	bool signedDistance(
			Sphere	*sphere,
		);

	public:

		vec3f		position;

};

#endif /* __RC_POINT_H__ */

