#include "ecosystem.h"


float VegetationSystem::DensityFromSlope(const Specie& s, float slope)
{
	return 0;
}

float VegetationSystem::DensityFromAltitude(const Specie& s, float altitude)
{
	return 0;
}

float VegetationSystem::DensityFromWetness(const Specie& s, float wetness)
{
	return 0;
}

float VegetationSystem::ComputeDensity(const Specie& s, float altitude, float slope, float wetness)
{
	return Min(DensityFromWetness(s, wetness), Min(DensityFromSlope(s, slope), DensityFromAltitude(s, altitude)));
}

Specie VegetationSystem::PineTreeSpecie()
{
	Specie pinetree;
	pinetree.altitudeLimits = Vector2(700.0f, 2500.0f);
	pinetree.slopeLimits = Vector2(0.0f, 0.7f);
	pinetree.wetnessLimits = Vector2(0.0f, 1.0f);
	pinetree.averageHeightData = Vector2(10.0f, 3.0f);
	pinetree.averageRadiusData = Vector2(8.0f, 3.0f);
	return pinetree;
}

Specie VegetationSystem::BroadleafSpecie()
{
	Specie broadleaf;
	broadleaf.altitudeLimits = Vector2(0.0f, 1000.0f);
	broadleaf.slopeLimits = Vector2(0.0f, 0.4f);
	broadleaf.wetnessLimits = Vector2(0.0f, 1.0f);
	broadleaf.averageHeightData = Vector2(6.0f, 1.5f);
	broadleaf.averageRadiusData = Vector2(4.0f, 1.1f);
	return broadleaf;
}
