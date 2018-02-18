#pragma once
#include "vec.h"

/* Perlin Noise s*/
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
	float Fbm(const Vector3& p, float, float, int);
	float Fbm(const Vector2& p, float, float, int);
};

/* Value Noise */
class ValueNoise
{

};

/* Gradient Noise */
class GradientNoise
{

};

/* Simplex Noise */
class SimplexNoise 
{
public:
	SimplexNoise() {}
	~SimplexNoise() {}
	double At(const Vector2&) const;
	double At(const Vector3&) const;
	double At(const Vector3&, const double&) const;

private:
	double dot(const int*, const double&, const double&) const;
	double dot(const int*, const double&, const double&, const double&) const;
	double dot(const int*, const double&, const double&, const double&, const double&) const;

	static const int grad2[8][2];		//!< Array of gradients for 2D noise.
	static const int grad3[12][3];		//!< Array of gradients for 3D noise.
	static const int grad4[32][4];		//!< Array of gradients for 4D noise.
	static const int perm[512];			//!< Permutation table, 256 entries duplicated once to avoid modulo computations.
	static const int simplex[64][4];	//!< Simplex data for 4D noise.
};