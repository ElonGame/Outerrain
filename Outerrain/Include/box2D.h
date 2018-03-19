#pragma once
#include "vec.h"
#include "box.h"

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
	void Scale(float f);
	Box2D Scaled(float f) const;

	Vector2 Vertex(int i) const;
	Vector2 Center() const;
	Vector2 BottomLeft() const;
	Vector2 TopRight() const;

	Box ToBox(double zMin, double zMax) const;
	Vector2& operator[](int i);

	friend std::ostream& operator<< (std::ostream& stream, const Box2D& b);
};
