#pragma once
#include "vec.h"

struct Specie
{
public:
	Vector2 altitudeLimits;
	Vector2 slopeLimits;
	Vector2 wetnessLimits;

	Vector2 averageHeightData;
	Vector2 averageRadiusData;

	Specie() { }
};

class VegetationSystem
{
protected:
	static float DensityFromSlope(const Specie& s, float slope);
	static float DensityFromAltitude(const Specie& s, float altitude);
	static float DensityFromWetness(const Specie& s, float wetness);

public:
	static float ComputeDensity(const Specie& s, float altitude, float slope, float wetness);

	static Specie PineTreeSpecie();
	static Specie BroadleafSpecie();
};
