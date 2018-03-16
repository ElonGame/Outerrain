#pragma once
#include "vec.h"

class Sphere
{
private:
	Vector3 center;
	float radius;

public:
	Sphere(const Vector3& c, float r);

	void Translate(const Vector3& T);
	float Distance(const Vector3& p) const;
	bool Contains(const Vector3& p) const;
	Vector3 Center() const;
	float Radius() const;

	friend std::ostream& operator<< (std::ostream& stream, const Sphere& b);
};
