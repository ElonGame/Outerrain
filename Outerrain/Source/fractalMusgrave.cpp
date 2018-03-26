#include <fractal.h>

/*
\brief All these functions are copied/pasted from Musgrave article :
https://www.classes.cs.uchicago.edu/archive/2015/fall/23700-1/final-project/MusgraveTerrain00.pdf

And adapted a little to match with C++.
*/

static float* exponent_array = nullptr;

static void InitExponentArrays(float lacunarity, float octaves, float H)
{
	delete exponent_array;

	float frequency;
	exponent_array = new float[static_cast<int>(octaves) + 1];
	frequency = 1.0;
	for (int i = 0; i <= octaves; i++)
	{
		exponent_array[i] = pow(frequency, -H);
		frequency *= lacunarity;
	}
}

/*
 * Procedural fBm evaluated at "point"; returns value stored in "value".
 *
 * Copyright 1994 F. Kenton Musgrave
 *
 * Parameters:
 *    ``H''  is the fractal increment parameter
 *    ``lacunarity''  is the gap between successive frequencies
 *    ``octaves''  is the number of frequencies in the fBm
 */
float Fractal::MusgravefBm(const Noise& n, Vector3 point, float H, float lacunarity, float octaves)
{
	InitExponentArrays(lacunarity, octaves, H);

	float value = 0.0;
	float frequency = 1.0;
	int i = 0;
	for (; i < octaves; i++)
	{
		value += n.GetValue(point) * exponent_array[i];
		point.x *= lacunarity;
		point.y *= lacunarity;
		point.z *= lacunarity;
	}

	float remainder = octaves - static_cast<int>(octaves);
	if (remainder)
		value += remainder * n.GetValue(point) * exponent_array[i];
	return value;
}

/*
 * Heterogeneous procedural terrain function: stats by altitude method.
 * Evaluated at "point"; returns value stored in "value".
 *
 * Copyright 1994 F. Kenton Musgrave
 *
 * Parameters:
 *       ``H''  determines the fractal dimension of the roughest areas
 *       ``lacunarity''  is the gap between successive frequencies
 *       ``octaves''  is the number of frequencies in the fBm
 *       ``offset''  raises the terrain from `sea level'
 */
float Fractal::MusgraveHeteroTerrain(const Noise& n, Vector3 point, float H, float lacunarity, float octaves, float offset)
{
	InitExponentArrays(lacunarity, octaves, H);

	/* first unscaled octave of function; later octaves are scaled */
	float value = offset + n.GetValue(point);
	point.x *= lacunarity;
	point.y *= lacunarity;
	point.z *= lacunarity;

	/* spectral construction inner loop, where the fractal is built */
	int i = 0;
	float increment;
	for (i = 1; i < octaves; i++)
	{
		/* obtain displaced noise value */
		increment = n.GetValue(point) + offset;

		/* scale amplitude appropriately for this frequency */
		increment *= exponent_array[i];

		/* scale increment by current `altitude' of function */
		increment *= value;

		/* add increment to ``value''  */
		value += increment;

		/* raise spatial frequency */
		point.x *= lacunarity;
		point.y *= lacunarity;
		point.z *= lacunarity;
	}

	/* take care of remainder in ``octaves''  */
	float remainder = octaves - static_cast<int>(octaves);
	if (remainder)
	{
		increment = (n.GetValue(point) + offset) * exponent_array[i];
		value += remainder * increment * value;
	}

	return value;
}

/* Hybrid additive/multiplicative multifractal terrain model.
 *
 * Copyright 1994 F. Kenton Musgrave
 *
 * Some good parameter values to start with:
 *
 *      H:           0.25
 *      offset:      0.7
 */
float Fractal::MusgraveHybridMultifractal(const Noise& n, Vector3 point, float H, float lacunarity, float octaves, float offset)
{
	InitExponentArrays(lacunarity, octaves, H);

	/* get first octave of function */
	float result = (n.GetValue(point) + offset) * exponent_array[0];
	float weight = result;

	/* increase frequency */
	point.x *= lacunarity;
	point.y *= lacunarity;
	point.z *= lacunarity;

	/* spectral construction inner loop, where the fractal is built */
	int i = 1;
	for (; i < octaves; i++)
	{
		/* prevent divergence */
		if (weight > 1.0)  weight = 1.0;

		/* get next higher frequency */
		float signal = (n.GetValue(point) + offset) * exponent_array[i];

		/* add it in, weighted by previous freq's local value */
		result += weight * signal;

		/* update the (monotonically decreasing) weighting value */
		/* (this is why H must specify a high fractal dimension) */
		weight *= signal;

		/* increase frequency */
		point.x *= lacunarity;
		point.y *= lacunarity;
		point.z *= lacunarity;
	}

	/* take care of remainder in ``octaves''  */
	float remainder = octaves - static_cast<int>(octaves);
	if (remainder)
		result += remainder * n.GetValue(point) * exponent_array[i];
	return result;
}

/* Ridged multifractal terrain model.
 *
 * Copyright 1994 F. Kenton Musgrave
 *
 * Some good parameter values to start with:
 *
 *      H:           1.0
 *      offset:      1.0
 *      gain:        2.0
 */
float Fractal::MusgraveRidgedMultifractal(const Noise& n, Vector3 point, float H, float lacunarity, float octaves, float offset, float gain)
{
	InitExponentArrays(lacunarity, octaves, H);

	/* get first octave */
	float signal = n.GetValue(point);

	/* get absolute value of signal (this creates the ridges) */
	if (signal < 0.0)
		signal = -signal;

	/* invert and translate (note that "offset" should be ~= 1.0) */
	signal = offset - signal;

	/* square the signal, to increase "sharpness" of ridges */
	signal *= signal;

	/* assign initial values */
	float result = signal;
	float weight = 1.0;
	int i = 1;
	for (; i < octaves; i++)
	{
		/* increase the frequency */
		point.x *= lacunarity;
		point.y *= lacunarity;
		point.z *= lacunarity;
		/* weight successive contributions by previous signal */
		weight = signal * gain;
		if (weight > 1.0)
			weight = 1.0;
		if (weight < 0.0)
			weight = 0.0;
		signal = n.GetValue(point);
		if (signal < 0.0)
			signal = -signal;
		signal = offset - signal;
		signal *= signal;

		/* weight the contribution */
		signal *= weight;
		result += signal * exponent_array[i];
	}

	return result;
}
