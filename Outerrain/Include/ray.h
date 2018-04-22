#pragma once

#include "vec.h"

typedef struct Ray
{
public:
	Vector3 origin;
	Vector3 direction;

	Ray() : origin(Vector3(0)), direction(Vector3(0)) { }
	Ray(const Vector3& origin, const Vector3& direction) : origin(origin), direction(direction) { }

	Vector3 At(float t) const { return origin + direction * t; }
	friend std::ostream& operator<<(std::ostream& stream, const Ray& u);
} Ray;

inline std::ostream& operator<<(std::ostream& stream, const Ray& u)
{
	stream << u.origin << " , " << u.direction << std::endl;
	return stream;
}

typedef struct Hit
{
public:
	Vector3 position;
	Vector3 normal;

	Hit() : position(Vector3(0)), normal(Vector3(0)) { }
	Hit(const Vector3& position, const Vector3& normal) : position(position), normal(normal) { }
	friend std::ostream& operator<<(std::ostream& stream, const Hit& u);
} Hit;

inline std::ostream& operator<<(std::ostream& stream, const Hit& u)
{
	stream << u.position << " , " << u.normal << std::endl;
	return stream;
}
