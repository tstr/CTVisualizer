/*
	Raycasting utilities:

	Intersection function definitions
*/

#include <algorithm>

#include "RayCasting.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RaycastResult Raycast::intersects(const AABB& box, const Ray& ray, size_t sampleFrequency)
{
	using namespace std;

	float tmin = (box.min.x() - ray.origin.x()) / ray.dir.x();
	float tmax = (box.max.x() - ray.origin.x()) / ray.dir.x();

	if (tmin > tmax) swap(tmin, tmax);

	float tymin = (box.min.y() - ray.origin.y()) / ray.dir.y();
	float tymax = (box.max.y() - ray.origin.y()) / ray.dir.y();

	if (tymin > tymax) swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return RaycastResult::nohit();

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (box.min.z() - ray.origin.z()) / ray.dir.z();
	float tzmax = (box.max.z() - ray.origin.z()) / ray.dir.z();

	if (tzmin > tzmax) swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return RaycastResult::nohit();

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	return RaycastResult(ray, tmin, tmax, 1.0f / sampleFrequency);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
