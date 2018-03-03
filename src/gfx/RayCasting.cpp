/*
	Raycasting utilities:

	Intersection function definitions
*/

#include <algorithm>

#include "RayCasting.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	min/max vector helpers
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QVector3D min(const QVector3D& a, const QVector3D& b)
{
	return QVector3D(std::min(a.x(), b.x()), std::min(a.y(), b.y()), std::min(a.z(), b.z()));
}

QVector3D max(const QVector3D& a, const QVector3D& b)
{
	return QVector3D(std::max(a.x(), b.x()), std::max(a.y(), b.y()), std::max(a.z(), b.z()));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RaycastResult Raycast::intersects(const AABB& box, const Ray& ray, size_t sampleFrequency)
{
	using namespace std;

	//intersection vectors with planes
	QVector3D tmin = (box.min - ray.origin.toVector3D()) / ray.dir;
	QVector3D tmax = (box.max - ray.origin.toVector3D()) / ray.dir;

	QVector3D tnear = min(tmin, tmax);
	QVector3D tfar = max(tmin, tmax);

	//entry point
	float t0 = max(max(tnear.x(), 0.0f), max(tnear.y(), tnear.z()));
	//exit point
	float t1 = min(tfar.x(), min(tfar.y(), tfar.z()));

	//If intersects
	if ((t1 > 0.0f) && (t0 < t1))
	{
		return RaycastResult(ray, t0, t1, 1.0f / sampleFrequency);
	}

	return RaycastResult::nohit();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
