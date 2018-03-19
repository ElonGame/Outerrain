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
bool Box::Intersect(const Ray& r)
{
	float tmin = (a.x - r.origin.x) / r.direction.x;
	float tmax = (b.x - r.origin.x) / r.direction.x;

	if (tmin > tmax) 
		Math::Swap(tmin, tmax);
	float tymin = (a.y - r.origin.y) / r.direction.y;
	float tymax = (b.y - r.origin.y) / r.direction.y;

	if (tymin > tymax) 
		Math::Swap(tymin, tymax);
	if ((tmin > tymax) || (tymin > tmax))
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (a.z - r.origin.z) / r.direction.z;
	float tzmax = (b.z - r.origin.z) / r.direction.z;
	if (tzmin > tzmax) 
		Math::Swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	return true;
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