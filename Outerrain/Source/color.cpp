#include "color.h"

Color::Color(float R, float G, float B, float A) : r(R), g(G), b(B), a(A)
{
}

Color::Color(const Color& color, float alpha) : r(color.r), g(color.g), b(color.b), a(alpha)
{
}

Color Color::Black()
{
	return Color(0, 0, 0);
}

Color Color::White()
{
	return Color(1, 1, 1);
}

Color Color::Red()
{
	return Color(1, 0, 0);
}

Color Color::Green()
{
	return Color(0, 1, 0);
}

Color Color::Blue()
{
	return Color(0, 0, 1);
}

Color Color::Grey()
{
	return Color(0.7f, 0.7f, 0.7f);
}

Color Color::operator+(const Color& c)
{
	return Color(r + c.r, g + c.g, b + c.b, a + c.a);
}

Color Color::operator-(const Color& c)
{
	return Color(r - c.r, g - c.g, b - c.b, a - c.a);
}

Color Color::operator*(const Color& c)
{
	return Color(r * c.r, g * c.g, b * c.b, a * c.a);
}

Color Color::operator*(float k)
{
	return Color(r * k, g * k, b * k, a * k);
}

Color Color::operator/(const Color& c)
{
	return Color(r / c.r, g / c.g, b / c.b, a / c.a);
}

Color Color::operator/(float k)
{
	return Color(r / k, g / k, b / k, a / k);
}

bool Color::operator==(const Color& c)
{
	return (r == c.r && g == c.g && b == c.b && a == c.a);
}

bool Color::operator!=(const Color& c)
{
	return (r != c.r || g != c.g || b != c.b || a != c.a);
}
