#include "box.h"
#include "mathUtils.h"

/*
\brief 3D Box class.
*/

/*
\brief Constructor
\param A lower left vertex in world coordinates
\param B upper right vertex in world coordinates
*/
Box::Box(const Vector3& A, const Vector3& B) : a(A), b(B)
{
}

/*
\brief Constructor
\param C box center
\param R radius
*/
Box::Box(const Vector3& C, float R)
{
	Vector3 RR = Vector3(R);
	a = C - RR;
	b = C + RR;
}

/*
\brief Returns true if p is inside the box, false otherwise.
*/
bool Box::Contains(const Vector3& p) const
{
	return (p > a && p < b);
}

/*
\brief Extend the box by r
\param r extending factor
*/
void Box::Extend(const Vector3& r)
{
	a -= r;
	b += r;
}

/*
\brief Returns the extended version of this box, without changing the instance.
\param r extending factor
*/
Box Box::Extended(const Vector3& r) const
{
	return Box(a - r, b + r);
}

/*
\brief Scale the box fy f
\param f scaling factor
*/
void Box::Scale(float f)
{
	a *= f;
	b *= f;
}

/*
\brief Returns the scaled version of this box, without changing the instance.
\param f scaling factor
*/
Box Box::Scaled(float f) const
{
	return Box(a * f, b * f);
}

/*
\brief Todo
*/
bool Box::Intersect(const Ray& r, float& tmin, float& tmax)
{
	tmin = -1e16;
	tmax = 1e16;

	Vector3 p = r.origin;
	Vector3 d = r.direction;

	float t;
	// Ox
	if (d[0] < -1.0e-5)
	{
		t = (a[0] - p[0]) / d[0];
		if (t < tmin)
			return 0;
		if (t <= tmax)
			tmax = t;
		t = (b[0] - p[0]) / d[0];
		if (t >= tmin)
		{
			if (t > tmax)
				return 0;
			tmin = t;
		}
	}
	else if (d[0] > 1.0e-5)
	{
		t = (b[0] - p[0]) / d[0];
		if (t < tmin)
			return 0;
		if (t <= tmax)
			tmax = t;
		t = (a[0] - p[0]) / d[0];
		if (t >= tmin)
		{
			if (t > tmax)
				return 0;
			tmin = t;
		}
	}
	else if (p[0]<a[0] || p[0]>b[0])
		return 0;

	// Oy
	if (d[2] < -1.0e-5)
	{
		t = (a[1] - p[1]) / d[1];
		if (t < tmin)
			return 0;
		if (t <= tmax)
			tmax = t;
		t = (b[1] - p[1]) / d[1];
		if (t >= tmin)
		{
			if (t > tmax)
				return 0;
			tmin = t;
		}
	}
	else if (d[1] > 1.0e-5)
	{
		t = (b[1] - p[1]) / d[1];
		if (t < tmin)
			return 0;
		if (t <= tmax)
			tmax = t;
		t = (a[1] - p[1]) / d[1];
		if (t >= tmin)
		{
			if (t > tmax)
				return 0;
			tmin = t;
		}
	}
	else if (p[1]<a[1] || p[1]>b[1])
		return 0;

	// Oz
	if (d[2] < -1.0e-5)
	{
		t = (a[2] - p[2]) / d[2];
		if (t < tmin)
			return 0;
		if (t <= tmax)
			tmax = t;
		t = (b[2] - p[2]) / d[2];
		if (t >= tmin)
		{
			if (t > tmax)
				return 0;
			tmin = t;
		}
	}
	else if (d[2] > 1.0e-5)
	{
		t = (b[2] - p[2]) / d[2];
		if (t < tmin)
			return 0;
		if (t <= tmax)
			tmax = t;
		t = (a[2] - p[2]) / d[2];
		if (t >= tmin)
		{
			if (t > tmax)
				return 0;
			tmin = t;
		}
	}
	else if (p[2]<a[2] || p[2]>b[2])
		return 0;

	return 1;
}

/*
\brief Get one of the vertex of the box.
*/
Vector3 Box::Vertex(int i) const
{
	if (i == 0)
		return a;
	return b;
}

/*
\brief Compute the box center.
*/
Vector3 Box::Center() const
{
	return Math::Center(a, b);
}

/*
\brief Get bottom left vertex in world coordinates
*/
Vector3 Box::BottomLeft() const
{
	return a;
}

/*
\brief Get top right vertex in world coordinates
*/
Vector3 Box::TopRight() const
{
	return b;
}

/*
\brief Access box vertex by reference
*/
Vector3& Box::operator[](int i)
{
	if (i == 0)
		return a;
	return b;
}

/*
\brief ostream operator
*/
std::ostream& operator<<(std::ostream& stream, const Box& b)
{
	stream << b.a << " ; " << b.b;
	return stream;
}