#pragma once
#include "vec.h"
#include "ray.h"

class Box
{
protected:
    Vector3 a;
    Vector3 b;

public:
    Box(const Vector3& A, const Vector3& B);
    Box(const Vector3& C, float R);

    bool Contains(const Vector3&) const;
    void Extend(const Vector3&);
    Box Extended(const Vector3&) const;
    void Scale(float);
    Box Scaled(float) const;

	bool Intersect(const Ray& ray, float& tmin, float& tmax);
    Vector3 Vertex(int) const;
    Vector3 Center() const;
    Vector3 BottomLeft() const;
    Vector3 TopRight() const;

	Vector3& operator[](int i);

	friend std::ostream& operator<< (std::ostream& stream, const Box& b);
};