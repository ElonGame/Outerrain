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

	static float MusgravefBm(const Noise& n, Vector3 point, float H, float lacunarity, float octaves);
	static float MusgraveMultifractal(const Noise& n, Vector3 point, float H, float lacunarity, float octaves, float offset);
	static float MusgraveHeteroTerrain(const Noise& n, Vector3 point, float H, float lacunarity, float octaves, float offset);
	static float MusgraveHybridMultifractal(const Noise& n, Vector3 point, float H, float lacunarity, float octaves, float offset );
	static float MusgraveRidgedMultifractal(const Noise& n, Vector3 point, float H, float lacunarity, float octaves, float offset, float gain);
};
