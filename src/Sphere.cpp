#include "RenderChimp.h"
#include "Ray.h"
#include "Plane.h"
#include "Volume.h"
#include "AABox.h"
#include "Sphere.h"

/*---------------------------------------------------------------------------*/

void Sphere::setRadius(
		f32				nradius
	)
{
	radius = nradius;
	radiusSquared = nradius * nradius;

}

/*---------------------------------------------------------------------------*/

bool Sphere::encloses(
		vec3f			*point
	)
{
	return (*point - origin).length2() <= radiusSquared;
}

/*---------------------------------------------------------------------------*/

vec3f Sphere::getCentrum()
{
	return origin;
}

/*---------------------------------------------------------------------------*/

f32 Sphere::volume()
{
	return (4.0f * fPI * radiusSquared * radius) / 3.0f;
}

/*---------------------------------------------------------------------------*/

bool Sphere::intersect(
		Ray				*ray
	)
{
	/* TODO: Implement me! */
	return false;
}

/*---------------------------------------------------------------------------*/

bool Sphere::intersect(
		Ray				*ray,
		f32				*dist
	)
{
	/* TODO: Implement me! */
	*dist = 0.0f;
	return true;
}

/*---------------------------------------------------------------------------*/

bool Sphere::intersect(
		AABox			*aabox
	)
{
	return aabox->intersect(this);
}

/*---------------------------------------------------------------------------*/

bool Sphere::intersect(
		Sphere			*sphere
	)
{
	f32 r = sphere->radius + radius;
	r *= r;

	return (sphere->origin - origin).length2() <= r;
}

/*---------------------------------------------------------------------------*/

void Sphere::combine(
		Sphere			*sphere
	)
{
	/* TODO: Implement me! */
}

/*---------------------------------------------------------------------------*/

