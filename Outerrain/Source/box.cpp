#include "box.h"
#include "mathUtils.h"

Box::Box(const Vector3& A, const Vector3& B) : a(A), b(B)
{
}

Box::Box(const Vector3& C, double R)
{
    Vector3 RR = Vector3(R);
    a = C - RR;
    b = C + RR;
}

bool Box::Inside(const Vector3& p) const
{
    return (p > a && p < b);
}

void Box::Extend(const Vector3& r)
{
    a -= r;
    b += r;
}

Box Box::Extended(const Vector3& r) const
{
    return Box(a - r, b + r);
}

void Box::Scale(double f)
{
    a *= f;
    b *= f;
}

Box Box::Scaled(double f) const
{
    return Box(a * f, b * f);
}

Vector3 Box::Vertex(int i) const
{
    if (i == 0)
        return a;
    return b;
}

Vector3 Box::Center() const
{
    return Math::Center(a, b);
}

Vector3 Box::BottomLeft() const
{
    return a;
}

Vector3 Box::TopRight() const
{
    return b;
}
