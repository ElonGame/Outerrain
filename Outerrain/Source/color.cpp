#include "color.h"

float Color::power() const
{
	return r + g + b;
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

Color operator+ (const Color& a, const Color& b)
{
	return Color(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);
}

Color operator- (const Color& c)
{
	return Color(-c.r, -c.g, -c.b, -c.a);
}

Color operator- (const Color& a, const Color& b)
{
	return a + (-b);
}

Color operator* (const Color& a, const Color& b)
{
	return Color(a.r * b.r, a.g * b.g, a.b * b.b, a.a * b.a);
}

Color operator* (const float k, const Color& c)
{
	return Color(c.r * k, c.g * k, c.b * k, c.a * k);
}

Color operator* (const Color& c, const float k)
{
	return k * c;
}

Color operator/ (const Color& a, const Color& b)
{
	return Color(a.r / b.r, a.g / b.g, a.b / b.b, a.a / b.a);
}

Color operator/ (const float k, const Color& c)
{
	return Color(k / c.r, k / c.g, k / c.b, k / c.a);
}

Color operator/ (const Color& c, const float k)
{
	float kk = 1 / k;
	return kk * c;
}
