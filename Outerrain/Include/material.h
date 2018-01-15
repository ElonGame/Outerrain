#pragma once
#include "color.h"

struct Material
{
public:
	Color diffuse = Color::White();
	float shininess = 32.0;

	Material(Color d, float s) : diffuse(d), shininess(s)
	{
	}
};
