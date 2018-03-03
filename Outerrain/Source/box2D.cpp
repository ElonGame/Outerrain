#include "box.h"
#include "mathUtils.h"

Box2D::Box2D(const Vector2& A, const Vector2& B) : a(A), b(B)
{
}

Box2D::Box2D(const Vector2& C, double R)
{
    Vector2 RR = Vector2(R);
    a = C - RR;
    b = C + RR;
}

bool Box2D::Inside(const Vector2& p) const
{
    return (p > a && p < b);
}

void Box2D::Extend(const Vector2& r)
{
    a -= r;
    b += r;
}

Box2D Box2D::Extended(const Vector2& r) const
{
    return Box2D(a - r, b + r);
}

void Box2D::Scale(double f)
{
    a *= f;
    b *= f;
}

Box2D Box2D::Scaled(double f) const
{
    return Box2D(a * f, b * f);
}

Vector2 Box2D::Vertex(int i) const
{
    if (i == 0)
        return a;
    return b;
}

Vector2 Box2D::Center() const
{
    return (a + b) / 2;
}

Vector2 Box2D::BottomLeft() const
{
    return a;
}

Vector2 Box2D::TopRight() const
{
    return b;
}
