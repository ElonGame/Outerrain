#pragma once
#include "noise.h"

class Perlin : Noise
{
private:
	Perlin() {}
	Perlin(int x, int y, int z, int s) : Noise(x, y, z, s) {};

protected:

public:
	static double Perlin2DAt(double, double, double, int, double, double);
};
#pragma once
