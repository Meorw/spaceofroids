#include "RenderChimp.h"
#include "Plane.h"
#include "Ray.h"
#include "Volume.h"
#include "Sphere.h"
#include "AABox.h"
#include <math.h>

/*---------------------------------------------------------------------------*/

AABox::AABox() : Volume()
{
	//minCorner = vec3f(fMax, fMax, fMax);
	//maxCorner = vec3f(fMin, fMin, fMin);
#ifdef DEBUG
	minCorner = vec3f(fMax, fMax, fMax);
	maxCorner = vec3f(-fMax, -fMax, -fMax);
#endif
}

/*---------------------------------------------------------------------------*/

AABox::~AABox()
{

}

/*---------------------------------------------------------------------------*/

AABox::AABox(
		AABox			&box
	) : Volume()
{
	minCorner = box.minCorner;
	maxCorner = box.maxCorner;
}

/*---------------------------------------------------------------------------*/

AABox &AABox::operator=(
		const AABox		&b
	)
{
	minCorner = b.minCorner;
	maxCorner = b.maxCorner;
	return *this;
}

/*---------------------------------------------------------------------------*/

void AABox::include(
		vec3f			point
	)
{
	if (point.x > maxCorner.x)
		maxCorner.x = point.x;

	if (point.y > maxCorner.y)
		maxCorner.y = point.y;

	if (point.z > maxCorner.z)
		maxCorner.z = point.z;


	if (point.x < minCorner.x)
		minCorner.x = point.x;

	if (point.y < minCorner.y)
		minCorner.y = point.y;

	if (point.z < minCorner.z)
		minCorner.z = point.z;
}

/*---------------------------------------------------------------------------*/

vec3f AABox::centrum()
{
	return (maxCorner + minCorner) * 0.5f;
}

/*---------------------------------------------------------------------------*/

bool AABox::encloses(
		vec3f			*point
	)
{
	return point->x <= maxCorner.x && point->x >= minCorner.x &&
		point->y <= maxCorner.y && point->y >= minCorner.y &&
		point->z <= maxCorner.z && point->z >= minCorner.z;
}

/*---------------------------------------------------------------------------*/

f32 AABox::volume()
{
	vec3f diff;

	diff = maxCorner - minCorner;

	return diff.length2();
}

/*---------------------------------------------------------------------------*/

f32 AABox::area()
{
	vec3f diff;

	diff = maxCorner - minCorner;

	return 2.0f * (diff.x * diff.y + diff.y * diff.z + diff.z * diff.x);
}

/*---------------------------------------------------------------------------*/

bool AABox::intersect(
		Ray				*ray
	)
{
	vec3f l1, l2;
	vec3f invdir;

	f32 lmin1, lmax1;
	f32 lmin2, lmax2;

	f32 lmin, lmax;

	invdir.x = 1.0f / ray->direction.x;
	invdir.y = 1.0f / ray->direction.y;
	invdir.z = 1.0f / ray->direction.z;

	l1 = (minCorner - ray->origin) * invdir;
	l2 = (maxCorner - ray->origin) * invdir;

	if (l1.x < l1.y) {
		if (l1.x < l1.z) {
			lmin1 = l1.x;
			lmax1 = l1.y < l1.z ? l1.z : l1.y;
		} else {
			lmin1 = l1.z;
			lmax1 = l1.y < l1.x ? l1.x : l1.y;
		}
	} else if (l1.y < l1.z) {
		lmin1 = l1.y;
		lmax1 = l1.z < l1.x ? l1.x : l1.z;
	} else {
		lmin1 = l1.z;
		lmax1 = l1.x < l1.y ? l1.y : l1.x;
	}

	if (l2.x < l2.y) {
		if (l2.x < l2.z) {
			lmin2 = l2.x;
			lmax2 = l2.y < l2.z ? l2.z : l2.y;
		} else {
			lmin2 = l2.z;
			lmax2 = l2.y < l2.x ? l2.x : l2.y;
		}
	} else if (l2.y < l2.z) {
		lmin2 = l2.y;
		lmax2 = l2.z < l2.x ? l2.x : l2.z;
	} else {
		lmin2 = l2.z;
		lmax2 = l2.x < l2.y ? l2.y : l2.x;
	}

	lmin = smin2<f32>(lmin1, lmin2);
	lmax = smax2<f32>(lmax1, lmax2);

	return (lmax >= 0.0f && lmax >= lmin);

}

/*---------------------------------------------------------------------------*/

bool AABox::intersect(
		Ray				*ray,
		f32				*dist
	)
{
	return false;
}

/*---------------------------------------------------------------------------*/

bool AABox::intersect(
		Ray				*ray,
		vec3f			inv_dir,
		f32				*dist
	)
{
	static const f32 eps = 0.0001f;
	u32 i;
	f32 tmin, tmax;

	tmin = 0.0f;
	tmax = fMax;

	for (i = 0; i < 3; i++) {
		if (fabs(ray->direction.vec[i]) < eps) {
			if (ray->origin.vec[i] < minCorner.vec[i] ||
				ray->origin.vec[i] > maxCorner.vec[i]) {

				*dist = fMax;
				return false;
			}
		} else {
			f32 t1 = (minCorner.vec[i] - ray->origin.vec[i]) * inv_dir.vec[i];
			f32 t2 = (maxCorner.vec[i] - ray->origin.vec[i]) * inv_dir.vec[i];

			if (t1 > t2) {
				f32 tmp = t1;
				t1 = t2;
				t2 = tmp;
			}

			tmin = smax2<f32>(tmin, t1);
			tmax = smin2<f32>(tmax, t2);

			if (tmin > tmax) {
				*dist = fMax;

				return false;
			}

		}
	}

	*dist = tmin;
	return true;
}

/*---------------------------------------------------------------------------*/

bool AABox::intersect(
		AABox			*aabox
	)
{
	return !(aabox->maxCorner.x < minCorner.x || aabox->minCorner.x > maxCorner.x ||
		aabox->maxCorner.y < minCorner.y || aabox->minCorner.y > maxCorner.y ||
		aabox->maxCorner.z < minCorner.z || aabox->minCorner.z > maxCorner.z);
}

/*---------------------------------------------------------------------------*/

bool AABox::intersect(
		Sphere			*sphere
	)
{
	vec3f point_on_box = sphere->origin;
	f32 dist;

	point_on_box.clamp(minCorner, maxCorner);

	dist = (point_on_box - sphere->origin).length2();

	return dist <= sphere->radiusSquared;
}

/*---------------------------------------------------------------------------*/

void AABox::combine(
		AABox			*box
	)
{
	minCorner.x = smin2<f32>(box->minCorner.x, minCorner.x);
	minCorner.y = smin2<f32>(box->minCorner.y, minCorner.y);
	minCorner.z = smin2<f32>(box->minCorner.z, minCorner.z);

	maxCorner.x = smax2<f32>(box->maxCorner.x, maxCorner.x);
	maxCorner.y = smax2<f32>(box->maxCorner.y, maxCorner.y);
	maxCorner.z = smax2<f32>(box->maxCorner.z, maxCorner.z);
}

/*---------------------------------------------------------------------------*/

void AABox::combine(
		Sphere			*sphere
	)
{
	minCorner.x = smin2<f32>(sphere->origin.x - sphere->radius, minCorner.x);
	minCorner.y = smin2<f32>(sphere->origin.y - sphere->radius, minCorner.y);
	minCorner.z = smin2<f32>(sphere->origin.z - sphere->radius, minCorner.z);

	maxCorner.x = smax2<f32>(sphere->origin.x + sphere->radius, maxCorner.x);
	maxCorner.y = smax2<f32>(sphere->origin.y + sphere->radius, maxCorner.y);
	maxCorner.z = smax2<f32>(sphere->origin.z + sphere->radius, maxCorner.z);
}

/*---------------------------------------------------------------------------*/

vec3f AABox::getCornerPoint(
		u32				i
	)
{
	vec3f pt;

	if (i & 1) {
		pt.x = maxCorner.x;
	} else {
		pt.x = minCorner.x;
	}

	if (i & 2) {
		pt.y = maxCorner.y;
	} else {
		pt.y = minCorner.y;
	}

	if (i & 4) {
		pt.z = maxCorner.z;
	} else {
		pt.z = minCorner.z;
	}

	return pt;
}

/*---------------------------------------------------------------------------*/
