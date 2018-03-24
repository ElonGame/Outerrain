#pragma once
#include "vec.h"
#include "color.h"

class DirectionnalLight
{
protected:
	Vector3 direction;
	Color color;
	Color ambientColor;
	float strength;

public:
	DirectionnalLight(const Vector3& dir, const Color& c, const Color& a, float s);

	Vector3 Direction() const;
	Color GetColor() const;
	Color AmbientColor() const;
	float Strength() const;
};
