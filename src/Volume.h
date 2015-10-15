/*
 * IMPORTANT NOTE: Very new and very untested.
 * USE AT YOUR OWN RISK!!!
 */

#ifndef RC_VOLUME_H
#define RC_VOLUME_H

class AABox;
class Sphere;

/**
 * Describes a closed 3D volume
 */
class Volume {

	public:

		virtual f32 volume() = 0;

		virtual bool encloses(
				vec3f			*point
			) = 0;

		virtual bool intersect(
				Ray				*ray
			) = 0;

		virtual bool intersect(
				Ray				*ray,
				f32				*dist
			) = 0;

		virtual bool intersect(
				AABox			*aabox
			) = 0;

		virtual bool intersect(
				Sphere			*sphere
			) = 0;

};

#endif /* RC_VOLUME_H */

