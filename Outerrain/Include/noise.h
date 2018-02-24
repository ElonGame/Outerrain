#pragma once
#include "vec.h"

/* Abstract */
class Noise
{
public:
	Noise() { }
	virtual ~Noise() { }
	virtual float GetValue(const Vector2&) const = 0;
	virtual float GetValue(const Vector3&) const = 0;
};

/* Perlin Noise */
class PerlinNoise : public Noise
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
