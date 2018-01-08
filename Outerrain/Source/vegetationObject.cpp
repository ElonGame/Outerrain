#include "vegetationObject.h"
#include "gameobject.h"
#include <algorithm>
#include <math.h>
#include <sstream>
#include <string>


VegetationObject::VegetationObject()
{

}

GameObject* VegetationObject::GetGameObject(Specie s)
{
	Mesh* m = new Mesh(GL_TRIANGLES);

	std::stringstream ss;
	ss << "Data/Objs/" << s.name << rand() % 3 + 1 << ".obj";
	m->ReadMesh(ss.str().c_str());

	Shader shader;
	shader.InitFromFile("Shaders/Diffuse.glsl");
	m->SetShader(shader);
	m->SetMaterial(Material(Color::Green(), 0));
	GameObject* obj = new GameObject();
	obj->AddComponent(m);
	obj->RotateAround(Vector3(0.0f, 1.0f, 0.0f), rand() % 360);
	return obj;
}

/* Density functions */
float VegetationObject::HeightDensityFactor(const Specie s, float height)
{
	return 1.0f - (abs(height - s.heightData.x) / s.heightData.y);
}

float VegetationObject::SlopeDensityFactor(const Specie s, float slope)
{
	return 1.0f - (abs((slope * 90.0f) - s.slopeData.x) / s.slopeData.y);
}

float VegetationObject::WetnessDensityFactor(const Specie s, float wetness)
{
	return 1.0f - (abs((wetness * 100.0f) - s.wetnessData.x) / s.wetnessData.y);
}

void VegetationObject::SetRadius(float r)
{
	radius = r;
}

float VegetationObject::GetRadius()
{
	return radius;
}

float VegetationObject::ComputeDensityFactor(Specie s, float height, float slope, float wetness)
{
	float min1 = std::min(HeightDensityFactor(s, height), SlopeDensityFactor(s, slope));
	return std::min(min1, WetnessDensityFactor(s, wetness));
}
