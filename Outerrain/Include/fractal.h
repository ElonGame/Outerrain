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
	static float fBm(const Noise& n, const Vector3& point, float a, float f, int octaves);
	static float fBm(const Noise& n, const Vector2& point, float a, float f, int octaves);
	static float RidgeNoise(const Noise& n, const Vector3& point, float a, float f, int octaves);

	static double MusgravefBm(const Noise& n, Vector3 point, double H, double lacunarity, double octaves);
	static double MusgraveMultifractal(const Noise& n, Vector3 point, double H, double lacunarity, double octaves, double offset);
	static double MusgraveHeteroTerrain(const Noise& n, Vector3 point, double H, double lacunarity, double octaves, double offset);
	static double MusgraveHybridMultifractal(const Noise& n, Vector3 point, double H, double lacunarity, double octaves, double offset );
	static double MusgraveRidgedMultifractal(const Noise& n, Vector3 point, double H, double lacunarity, double octaves, double offset, double gain);
};
