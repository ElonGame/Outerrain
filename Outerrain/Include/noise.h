#pragma once

class Noise
{
protected:
	static int xNoise;
	static int yNoise;
	static int shiftNoise;
	static int seedNoise;

	Noise() {}
	Noise(int, int, int, int);

public:
	/* Noise 2D */
	static double ValueNoise2D(int, int, int);

	static double GradientNoise2D(double, double, int, int, int);
	static double GradientCoherentNoise2D(double, double, int seed = 0);

	/* Utilities Functions */
	static double LinearInterp(double, double, double);
	static double MakeInt32Range(double n);
};
#pragma once
