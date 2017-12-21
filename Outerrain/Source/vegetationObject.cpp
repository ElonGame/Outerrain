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
	if (s == PineTree)
		return cos(height * 1.6f) * 0.5f + 0.5f;
	return cos(height * 1.6f) * 0.5f + 0.5f;
}

float VegetationObject::SlopeDensityFactor(const Specie s, float slope)
{
	if (s == PineTree)
		return cos(slope * 0.05f) * 0.5f + 0.5f;
	return cos(slope * 0.05f) * 0.5f + 0.5f;
}

float VegetationObject::WetnessDensityFactor(const Specie s, float wetness)
{
	if (s == PineTree)
		return cos(wetness * 1.6f) * 0.5f + 0.5f;
	return cos(wetness * 1.6f) * 0.5f + 0.5f;
}


float VegetationObject::StreamPowerDensityFactor(const Specie s, float streamPower)
{
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
	float min2 = std::min(WetnessDensityFactor(s, wetness), StreamPowerDensityFactor(s, accessibilitys));
	return std::min(min1, min2);
}
