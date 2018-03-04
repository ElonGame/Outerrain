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
Box::Box(const Vector3& C, double R)
{
    Vector3 RR = Vector3(R);
    a = C - RR;
    b = C + RR;
}

/*
\brief Returns true if p is inside the box, false otherwise.
*/
bool Box::Inside(const Vector3& p) const
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
void Box::Scale(double f)
{
    a *= f;
    b *= f;
}

/*
\brief Returns the scaled version of this box, without changing the instance.
\param f scaling factor
*/
Box Box::Scaled(double f) const
{
    return Box(a * f, b * f);
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
