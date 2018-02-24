#pragma once
#include "noise.h"
#include "vec.h"

enum FractalType
{
	fBm = 0,
	Ridge = 1,
};

class Fractal
{
public:
	static float fBm(const PerlinNoise& n, const Vector3& point, float a, float f, int octaves);
	static float fBm(const PerlinNoise& n, const Vector2& point, float a, float f, int octaves);
	static float RidgeNoise(const PerlinNoise& n, const Vector3& point, float a, float f, int octaves);
};
