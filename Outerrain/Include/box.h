#pragma once
#include "vec.h"

class Box2D
{
protected:
    Vector2 a;
    Vector2 b;

public:
    Box2D(const Vector2& A, const Vector2& B);
    Box2D(const Vector2& C, float R);

    bool Contains(const Vector2&) const;
    void Extend(const Vector2&);
    Box2D Extended(const Vector2&) const;
    void Scale(float);
    Box2D Scaled(float) const;

    Vector2 Vertex(int) const;
    Vector2 Center() const;
    Vector2 BottomLeft() const;
    Vector2 TopRight() const;

	Vector2& operator[](int i);

	friend std::ostream& operator<< (std::ostream& stream, const Box2D& b);
};

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

    Vector3 Vertex(int) const;
    Vector3 Center() const;
    Vector3 BottomLeft() const;
    Vector3 TopRight() const;

	Vector3& operator[](int i);

	friend std::ostream& operator<< (std::ostream& stream, const Box& b);
};