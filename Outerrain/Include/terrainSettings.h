#pragma once

#include "vec.h"
#include "fractal.h"
#include "material.h"

enum TerrainType
{
	HeightFieldTerrain = 0,
	NoiseFieldTerrain = 1,
};

class TerrainSettings
{
public:
	TerrainType terrainType;
	MaterialType shaderType;

	int resolution;
	Vector2 bottomLeft;
	Vector2 topRight;

	float amplitude;
	float frequency;
	int octaves;
	Vector3 offsetVector;
	FractalType fractalType;
	Noise* noise;

	std::string filePath;
	float minAltitude;
	float maxAltitude;

	TerrainSettings()
	{
		noise = nullptr;
	}

	~TerrainSettings()
	{
		delete noise;
	}

	TerrainSettings(int N, const Vector2& bLeft, const Vector2& tRight, float A, float F, int O, const Vector3& o, FractalType t, Noise* n) 
					: resolution(N), bottomLeft(bLeft), topRight(tRight), amplitude(A), frequency(F), octaves(0), offsetVector(o), fractalType(t), noise(n)
	{
		terrainType = NoiseFieldTerrain;
	}

	TerrainSettings(int N, const Vector2& bLeft, const Vector2& tRight, const std::string& p, float min, float max)
					: resolution(N), bottomLeft(bLeft), topRight(tRight), filePath(p), minAltitude(min), maxAltitude(max)
	{
		terrainType = HeightFieldTerrain;
	}
};
