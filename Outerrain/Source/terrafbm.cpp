#include "terratree.h"
#include "fractal.h"

PerlinNoise TerrafBm::noise = PerlinNoise();

TerrafBm::TerrafBm(float a, float f, int o) : amplitude(a), frequency(f), octave(o)
{
}

float TerrafBm::Value(const Vector2& p) const
{
	return Fractal::fBm(TerrafBm::noise, p, amplitude, frequency, octave);
}
