#include "light.h"

/*
\brief
*/
DirectionnalLight::DirectionnalLight(const Vector3& d, const Color& c, const Color& a, float s) : direction(d), color(c), ambientColor(a), strength(s)
{
}

/*
\brief
*/
Vector3 DirectionnalLight::Direction() const
{
	return direction;
}

/*
\brief
*/
Color DirectionnalLight::GetColor() const
{
	return color;
}

/*
\brief
*/
Color DirectionnalLight::AmbientColor() const
{
	return ambientColor;
}

/*
\brief
*/
float DirectionnalLight::Strength() const
{
	return strength;
}
