/*
 * IMPORTANT NOTE: Very new and very untested.
 * USE AT YOUR OWN RISK!!!
 */

#ifndef RC_SPHERE_H
#define RC_SPHERE_H

class AABox;

class Sphere : public Volume {

	public:

		void setRadius(
				f32				nradius
			);

		void combine(
				Sphere			*sphere
			);

		/* From Volume.h */

		vec3f getCentrum();

		f32 volume();

		bool encloses(
				vec3f			*point
			);

		bool intersect(
				Ray				*ray
			);

		bool intersect(
				Ray				*ray,
				f32				*dist
			);

		bool intersect(
				AABox			*aabox
			);

		bool intersect(
				Sphere			*sphere
			);


	public:

		vec3f		origin;
		f32			radius;
		f32			radiusSquared;

};

#endif /* RC_SPHERE_H */

