/*
 * IMPORTANT NOTE: Very new and very untested.
 * USE AT YOUR OWN RISK!!!
 */

#ifndef RC_AABOX_H
#define RC_AABOX_H

class Sphere;

class AABox : public Volume {

	public:

		AABox();

		AABox(
				AABox			&box
			);
	
		~AABox();

		AABox &operator=(
				const AABox		&b
			);

		void include(
				vec3f			point
			);

		void combine(
				AABox			*aabox
			);

		void combine(
				Sphere			*sphere
			);

		vec3f centrum();

		vec3f getCornerPoint(
				u32				i
			);

		/* From Volume.h */

		f32 area();

		f32 volume();

		bool encloses(
				vec3f			*point
			);

		bool intersect(
				Ray				*ray
			);

		bool intersect(
				Ray				*ray,
				vec3f			inv_dir,
				f32				*dist
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

		vec3f		minCorner;
		vec3f		maxCorner;

};

#endif /* RC_AABOX_H */

