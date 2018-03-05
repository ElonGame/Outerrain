#pragma once

struct Color
{
public:
	float r, g, b, a;

	Color(float _r = 0.f, float _g = 0.f, float _b = 0.f, float _a = 1.f) : r(_r), g(_g), b(_b), a(_a) {}
	Color(const Color& color, float alpha) : r(color.r), g(color.g), b(color.b), a(alpha) {}

	Color operator+(const Color& b);
	Color operator-(const Color& c);
	Color operator*(float k);
	Color operator*(const Color& c);
	Color operator/(float kc);
	Color operator/(const Color& c);

	static Color Black();
	static Color White();
	static Color Red();
	static Color Green();
	static Color Blue();
	static Color Grey();
};
