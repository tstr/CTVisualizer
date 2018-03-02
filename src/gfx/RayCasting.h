/*
	Ray casting utilities
	
	Contains helpers for performing ray casting operations in a 3D scene
*/

#pragma once

#include <QVector3D>
#include <QVector4D>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
	Axis aligned bounding box
*/
struct AABB
{
	QVector3D max;
	QVector3D min;

	AABB() {}

	AABB(const QVector3D& _min, const QVector3D& _max) :
		max(_max), min(_min)
	{}
};


/*
	3D Ray
*/
struct Ray
{
	QVector4D origin;
	QVector3D dir;

	Ray() {}

	Ray(const QVector3D& _origin, const QVector3D& _dir) :
		origin(_origin), dir(_dir)
	{}

	Ray(const QVector4D& _origin, const QVector3D& _dir) :
		origin(_origin), dir(_dir)
	{}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
	Ray iterator

	Represents a point along a ray.
*/
class RayIterator : std::iterator<std::random_access_iterator_tag, QVector3D, size_t, QVector3D, QVector3D>
{
public:

	//Construct from a point, normalized direction and step
	RayIterator(const QVector3D& cur, const QVector3D& dir, float step) : 
		m_cur(cur), m_step(dir * step)
	{}

	//Construct from a point and (vector) step
	RayIterator(const QVector3D& cur, const QVector3D& step) :
		m_cur(cur), m_step(step)
	{}

	RayIterator(const RayIterator&) = default;

	//Increment ray
	RayIterator operator++(int) const { RayIterator temp(*this); ++temp; return temp; }
	RayIterator& operator++() { m_cur += m_step; return *this; }

	//Arithmetic operators
	RayIterator operator-(RayIterator s) { return RayIterator(m_cur - s.m_cur, m_step); }
	RayIterator operator+(RayIterator s) { return RayIterator(m_cur + s.m_cur, m_step); }
	RayIterator& operator+=(size_t s) { m_cur += (m_step * s); return *this; }
	RayIterator& operator-=(size_t s) { m_cur -= (m_step * s); return *this; }

	//Access current point
	QVector3D operator*() const { return m_cur; }

	//Relational operations
	bool operator==(const RayIterator& rhs) const {
		//Two ray points are considered equal when they are near enough to each other
		return m_cur.distanceToPoint(rhs.m_cur) <= m_step.length();
	}

	bool operator!=(const RayIterator& rhs) const { return !this->operator==(rhs); }

private:

	QVector3D m_cur;
	QVector3D m_step;
};

/*
	Raycast result structure

	Stores the results of an intersection test between a ray and some shape
*/
class RaycastResult
{
public:

	using iterator = RayIterator;

	/*
		Construct ray-cast result structure from a ray, two intersection points and a step
	*/
	RaycastResult(const Ray& ray, float startD, float endD, float step) :
		m_ray(ray),
		m_step(step)
	{
		m_start = m_ray.origin.toVector3D() + (startD * m_ray.dir);
		m_end   = m_ray.origin.toVector3D() + (endD   * m_ray.dir);
	}

	/*
		Iterators
	*/
	iterator begin() const { return RayIterator(m_start, m_ray.dir, m_step); }
	iterator end()   const { return RayIterator(m_end,   m_ray.dir, m_step); }

	/*
		True if the ray cast has intersected something
	*/
	bool hit() const { return begin() != end(); }
	operator bool() const { return hit(); }

	/*
		No hit static helper
	*/
	static RaycastResult nohit()
	{
		return RaycastResult(Ray(QVector3D(), QVector3D()), 0.0f, 0.0f, 0.0f);
	}

private:

	//Start ending intersection points of ray
	QVector3D m_start;
	QVector3D m_end;

	float m_step;

	//Ray
	Ray m_ray;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
	Ray cast class
*/
class Raycast
{
public:

	/*
		Perform an intersection test on a given Bounding Box and a ray

		A sampling frequency can be specified
	*/
	static RaycastResult intersects(const AABB& box, const Ray& ray, size_t sampleFrequency = 100);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
