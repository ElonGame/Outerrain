#include "box2D.h"
#include "mathUtils.h"

/*
\brief 2D Box class.
*/

/*
\brief Constructor
\param A lower left vertex in world coordinates
\param B upper right vertex in world coordinates
*/
Box2D::Box2D(const Vector2& A, const Vector2& B) : a(A), b(B)
{
}

/*
\brief Constructor
\param C box center
\param R radius
*/
Box2D::Box2D(const Vector2& C, float R)
{
    Vector2 RR = Vector2(R);
    a = C - RR;
    b = C + RR;
}

/*
\brief Returns true if p is inside the box, false otherwise.
*/
bool Box2D::Contains(const Vector2& p) const
{
    return (p > a && p < b);
}

/*
\brief Extend the box by r
\param r extending factor
*/
void Box2D::Extend(const Vector2& r)
{
    a -= r;
    b += r;
}

/*
\brief Returns the extended version of this box, without changing the instance.
\param r extending factor
*/
Box2D Box2D::Extended(const Vector2& r) const
{
    return Box2D(a - r, b + r);
}

/*
\brief Scale the box fy f
\param f scaling factor
*/
void Box2D::Scale(float f)
{
    a *= f;
    b *= f;
}

/*
\brief Returns the scaled version of this box, without changing the instance.
\param f scaling factor
*/
Box2D Box2D::Scaled(float f) const
{
    return Box2D(a * f, b * f);
}

/*
\brief Get one of the vertex of the box.
*/
Vector2 Box2D::Vertex(int i) const
{
    if (i == 0)
        return a;
    return b;
}

/*
\brief Compute the box center.
*/
Vector2 Box2D::Center() const
{
    return (a + b) / 2;
}

/*
\brief Get bottom left vertex in world coordinates
*/
Vector2 Box2D::BottomLeft() const
{
    return a;
}

/*
\brief Get top right vertex in world coordinates
*/
Vector2 Box2D::TopRight() const
{
    return b;
}

/*
\brief Todo
*/
Box Box2D::ToBox(float zMin, float zMax) const
{
	return Box(Vector3(a.x, a.y, zMin), Vector3(b.x, b.y, zMax));
}

/*
\brief Access box vertex by reference
*/
Vector2& Box2D::operator[](int i)
{
	if (i == 0)
		return a;
	return b;
}

/*
\brief ostream operator
*/
std::ostream& operator<<(std::ostream& stream, const Box2D& b)
{
	stream << b.a << " ; " << b.b;
	return stream;
}
