#include "fractal.h"

/*
\brief Various custom fractal are implemented in this file, mostly from https://ordinatous.com/pdf/The_Fractal_Geometry_of_Nature.pdf.
*/

/*
\brief 3D Fractional Brownian motion
If you want to translate the noise, translate the point parameters before calling the function.
\param n noise used for the fractal
\param point point in 3D
\param a amplitude
\param f frequency
\param octave octave count
*/
float Fractal::fBm(const Noise& n, const Vector3& point, float a, float f, int octaves)
{
	float ret = 0.0f;
	float freq = f;
	float amp = a;
	for (int i = 0; i < octaves; i++)
	{
		ret += n.GetValue(point * freq) * amp;
		amp *= 0.5f;
		freq *= 2.0f;
	}
	return ret;
}

/*
\brief 2D Fractional Brownian motion
If you want to translate the noise, translate the point parameters before calling the function.
\param n noise used for the fractal
\param point point in 2D
\param a noise amplitude
\param f noise frequency
\param octave octave count
*/
float Fractal::fBm(const Noise& n, const Vector2& point, float a, float f, int octaves)
{
	float ret = 0.0f;
	float freq = f;
	float amp = a;
	for (int i = 0; i < octaves; i++)
	{
		ret += n.GetValue(point * freq) * amp;
		amp *= 0.5f;
		freq *= 2.0f;
	}
	return ret;
}

/*
\brief Simple implementation of Ridge noise, which is defined as abs(noise(p)) * -1.0f.
If you want to translate the noise, translate the point parameters before calling the function.
\param n noise used for the fractal
\param point point in 3D
\param a noise amplitude
\param f noise frequency
\param octaves octave count
*/
float Fractal::RidgeNoise(const Noise& n, const Vector3& point, float a, float f, int octaves)
{
	float ret = 0.0f;
	float freq = f;
	float amp = a;
	for (int i = 0; i < octaves; i++)
	{
		ret += amp * fabs(n.GetValue(point * freq)) * -1.0f;
		amp *= 0.5f;
		freq *= 2.0f;
	}
	return ret;
}
