#pragma once
#include "vec.h"

class PerlinNoise
{
private:
	int *p;
	float *Gx;
	float *Gy;
	float *Gz;
public:
	PerlinNoise();
	~PerlinNoise();
	float GetValue(const Vector2&);
	float Fbm(const Vector2& p, float, float, int);
};
