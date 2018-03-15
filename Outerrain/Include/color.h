#pragma once

struct Color
{
public:
	float r, g, b, a;

	explicit Color(float R = 0.f, float G = 0.f, float B = 0.f, float A = 1.f);
	explicit Color(const Color& color, float alpha);

	Color operator+(const Color& b);
	Color operator-(const Color& c);
	Color operator*(float k);
	Color operator*(const Color& c);
	Color operator/(float kc);
	Color operator/(const Color& c);
	bool operator==(const Color& c);
	bool operator!=(const Color& c);

	static Color Black();
	static Color White();
	static Color Red();
	static Color Green();
	static Color Blue();
	static Color Grey();
};
