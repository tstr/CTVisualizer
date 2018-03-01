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

	//Compute intersection point with x planes
	float tmin = (box.min.x() - ray.origin.x()) / ray.dir.x();
	float tmax = (box.max.x() - ray.origin.x()) / ray.dir.x();

	//Ensure tmax > tmin
	if (tmin > tmax) swap(tmin, tmax);

	//Compute intersection point with y planes
	float tymin = (box.min.y() - ray.origin.y()) / ray.dir.y();
	float tymax = (box.max.y() - ray.origin.y()) / ray.dir.y();

	//Ensure tymax > tymin
	if (tymin > tymax) swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return RaycastResult::nohit();

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	//Compute intersection point with z planes
	float tzmin = (box.min.z() - ray.origin.z()) / ray.dir.z();
	float tzmax = (box.max.z() - ray.origin.z()) / ray.dir.z();

	//Ensure tzmax > tzmin
	if (tzmin > tzmax) swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return RaycastResult::nohit();

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	//Clamp intersection distances along ray
	//tmin/tmax might be less than zero if the ray origin is inside the AABB
	tmin = std::max(0.0f, tmin);
	tmax = std::max(0.0f, tmax);

	return RaycastResult(ray, tmin, tmax, 1.0f / sampleFrequency);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
