#pragma once

#include "vec.h"

typedef struct Ray
{
public:
	Vector3 origin;
	Vector3 direction;

	Ray() : origin(Vector3(0)), direction(Vector3(0)) {}
	Ray(const Vector3& origin, const Vector3& direction) : origin(origin), direction(direction) {}
} Ray;

typedef struct Hit
{
public:
	Vector3 position;
	Vector3 normal;

	Hit() : position(Vector3(0)), normal(Vector3(0)) {}
	Hit(const Vector3& position, const Vector3& normal) : position(position), normal(normal) {}
} Hit;
