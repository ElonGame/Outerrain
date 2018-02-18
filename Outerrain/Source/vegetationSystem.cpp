#include "vegetationSystem.h"


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
	pinetree.altitudeLimits = Vector2(700, 2500);
	pinetree.slopeLimits = Vector2(0.0, 0.7);
	pinetree.wetnessLimits = Vector2(0.0, 1.0);
	pinetree.averageHeightData = Vector2(10, 3);
	pinetree.averageRadiusData = Vector2(8, 3);
	return pinetree;
}

Specie VegetationSystem::BroadleafSpecie()
{
	Specie broadleaf;
	broadleaf.altitudeLimits = Vector2(0, 1000);
	broadleaf.slopeLimits = Vector2(0.0, 0.4);
	broadleaf.wetnessLimits = Vector2(0.0, 1.0);
	broadleaf.averageHeightData = Vector2(6, 1.5);
	broadleaf.averageRadiusData = Vector2(4, 1.1);
	return broadleaf;
}
