#pragma once
#include "vec.h"

/* Perlin Noise */
class PerlinNoise
{
private:
	int* p;
	float* Gx;
	float* Gy;
	float* Gz;

	float At(const Vector3&) const;

public:
	PerlinNoise();
	~PerlinNoise();

	float GetValue(const Vector2&) const;
	float GetValue(const Vector3&) const;
};
