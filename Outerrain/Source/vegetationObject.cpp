#include "vegetationObject.h"
#include "gameobject.h"
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
	GameObject* obj = new GameObject();
	obj->AddComponent(m);
	obj->SetScale(1.0, 4.0, 1.0);
	return obj;
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

void VegetationObject::SetRadius(float r)
{
	radius = r;
}

float VegetationObject::GetRadius()
{
	return radius;
}
