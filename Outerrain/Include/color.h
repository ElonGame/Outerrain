#ifndef _COLOR_H
#define _COLOR_H

struct Color
{
	Color(const float _r = 0.f, const float _g = 0.f, const float _b = 0.f, const float _a = 1.f)
		: r(_r), g(_g), b(_b), a(_a) {}
	Color(const Color& color, const float alpha) 
		: r(color.r), g(color.g), b(color.b), a(alpha) {}

	float power() const;

	float r, g, b, a;

	static Color Black();
	static Color White();
	static Color Red();
	static Color Green();
	static Color Blue();
	static Color Grey();
};

Color operator+ (const Color& a, const Color& b);
Color operator- (const Color& a, const Color& b);
Color operator- (const Color& c);
Color operator* (const Color& a, const Color& b);
Color operator* (const Color& c, const float k);
Color operator* (const float k, const Color& c);
Color operator/ (const Color& a, const Color& b);
Color operator/ (const float k, const Color& c);
Color operator/ (const Color& c, const float k);

#endif
