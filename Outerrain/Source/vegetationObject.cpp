#include "vegetationObject.h"
#include <math.h>


VegetationObject::VegetationObject()
{

}

Mesh VegetationObject::GetMesh()
{
	Mesh m;
	// @Todo
	return m;
}


/* Density functions */
float VegetationObject::HeightDensityFactor(float height)
{
	return cos(height * 1.6);
}

float VegetationObject::SlopeDensityFactor(float slope)
{
	return cos(slope * 1.6);
}

float VegetationObject::StreamPowerDensityFactor(float streamPower)
{
	return cos(streamPower * 1.6);
}

float VegetationObject::WetnessDensityFactor(float wetness)
{
	return cos(wetness * 1.6);
}
