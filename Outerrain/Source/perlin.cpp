#include "perlin.h"

double Perlin::Perlin2DAt(double frequency, double lacunarity, double persistence, int octave, double x, double y)
{
	double value = 0.0;
	double curPersistence = 1.0;

	x *= frequency;
	y *= frequency;

	for (int i = 0; i < octave; i++)
	{
		double nx = MakeInt32Range(x);
		double ny = MakeInt32Range(y);

		int localSeed = seedNoise + i;
		double signal = GradientCoherentNoise2D(nx, ny, localSeed);
		value += signal * curPersistence;

		x *= lacunarity;
		y *= lacunarity;
		curPersistence *= persistence;
	}
	return value;
}