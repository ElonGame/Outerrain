#include "vegetationObject.h"
#include "gameobject.h"
#include <algorithm>
#include <math.h>

VegetationObject::VegetationObject()
{

}

GameObject* VegetationObject::GetGameObject()
{
	Mesh* m = new Mesh(GL_TRIANGLES);
	m->ReadMesh("Data/cube.obj");
	Shader shader;
	shader.InitFromFile("Shaders/Diffuse.glsl");
	m->SetShader(shader);
	m->SetMaterial(Material(Color::Green(), 64));
	GameObject* obj = new GameObject();
	obj->AddComponent(m);
	obj->SetScale(1.0, 4.0, 1.0);
	return obj;
}


/* Density functions */
float VegetationObject::HeightDensityFactor(float height)
{
	return cos(height * 1.6f) * 0.5f + 0.5f;
}

float VegetationObject::SlopeDensityFactor(float slope)
{
	return cos(slope * 0.05f) * 0.5f + 0.5f;
}

float VegetationObject::WetnessDensityFactor(float wetness)
{
	return cos(wetness * 1.6f) * 0.5f + 0.5f;
}


float VegetationObject::StreamPowerDensityFactor(float streamPower)
{
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

float VegetationObject::ComputeDensityFactor(float height, float slope, float wetness, float streampower)
{
	float min1 = std::min(HeightDensityFactor(height), SlopeDensityFactor(slope));
	float min2 = std::min(WetnessDensityFactor(wetness), StreamPowerDensityFactor(streampower));
	return std::min(min1, min2);
}