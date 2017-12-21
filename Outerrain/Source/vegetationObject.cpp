#include "vegetationObject.h"
#include "gameobject.h"
#include <algorithm>
#include <math.h>


VegetationObject::VegetationObject()
{

}

GameObject* VegetationObject::GetGameObject(Specie s)
{
	Mesh* m = new Mesh(GL_TRIANGLES);
	if (s == PineTree)
		m->ReadMesh("Data/Objs/pineTree.obj");
	else
		m->ReadMesh("Data/Objs/broadleaf.obj");
	Shader shader;
	shader.InitFromFile("Shaders/Diffuse.glsl");
	m->SetShader(shader);
	m->SetMaterial(Material(Color::Green(), 0));
	GameObject* obj = new GameObject();
	obj->AddComponent(m);
	return obj;
}


/* Density functions */
float VegetationObject::HeightDensityFactor(const Specie s, float height)
{
	float baseHeight;
	float rangeHeight;
	if (s == PineTree)
	{
		baseHeight = 50.0f;
		float maxHeight = 135.0f;
		rangeHeight = maxHeight - baseHeight;
	}
	else
	{
		baseHeight = 0.0f;
		float maxHeight = 30.0f;
		rangeHeight = maxHeight - baseHeight;
	}
	return 1.0f - (abs(height - baseHeight) / rangeHeight);
}

float VegetationObject::SlopeDensityFactor(const Specie s, float slope)
{
	float baseSlope;
	float rangeSlope;
	if (s == PineTree)
	{
		baseSlope = 15.0f;
		float maxSlope = 70.0f;
		rangeSlope = maxSlope - baseSlope;
	}
	else
	{
		baseSlope = 0.0f;
		float maxSlope = 35.0f;
		rangeSlope = maxSlope - baseSlope;
	}
	return 1.0f - (abs((slope * 90.0f) - baseSlope) / rangeSlope);
}

float VegetationObject::WetnessDensityFactor(const Specie s, float wetness)
{
	return 1.0f;
	if (s == PineTree)
		return cos(wetness * 1.6f) * 0.5f + 0.5f;
	return cos(wetness * 1.6f) * 0.5f + 0.5f;
}


float VegetationObject::AccessibilityDensityFactor(const Specie s, float streamPower)
{
	return 1.0f;
	if (s == PineTree)
		return cos(streamPower * 1.6f) * 0.5f + 0.5f;
	return cos(streamPower * 1.6f) * 0.5f + 0.5f;
}

void VegetationObject::SetRadius(float r)
{
	radius = r;
}

float VegetationObject::GetRadius()
{
	return radius;
}

float VegetationObject::ComputeDensityFactor(Specie s, float height, float slope, float wetness, float accessibilitys)
{
	float min1 = std::min(HeightDensityFactor(s, height), SlopeDensityFactor(s, slope));
	float min2 = std::min(WetnessDensityFactor(s, wetness), AccessibilityDensityFactor(s, accessibilitys));
	return std::min(min1, min2);
}