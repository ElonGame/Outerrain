#include <fractal.h>

/*
\brief All these functions are copied/pasted from Musgrave article :
https://www.classes.cs.uchicago.edu/archive/2015/fall/23700-1/final-project/MusgraveTerrain00.pdf 

Some work would be needed in order to make more of a C++ version, and factorize initialization of
all the exponent_array into a static function. Comments are from the original implementation.
*/

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
    float value, frequency, remainder, Noise3();
    int i;
    static bool first = true;
    static float *exponent_array;

    /* precompute and store spectral weights */
    if (first) 
    {
        /* seize required memory for exponent_array */
        exponent_array = (float*)malloc((octaves+1) * sizeof(float));
        frequency = 1.0;
        for (i=0; i<=octaves; i++) 
        {
            exponent_array[i] = pow( frequency, -H );
            frequency *= lacunarity;
        }
        first = false;
    }

    value = 0.0;
    frequency = 1.0;

    /* inner loop of spectral construction */
    for (i=0; i<octaves; i++) 
    {
        value += n.GetValue(point) * exponent_array[i];
        point.x *= lacunarity;
        point.y *= lacunarity;
        point.z *= lacunarity;
    }

    remainder = octaves - (int)octaves;
    if ( remainder )
        value += remainder * n.GetValue( point ) * exponent_array[i];

    return value;
}


/*
 * Procedural multifractal evaluated at "point"; 
 * returns value stored in "value".
 *
 * Copyright 1994 F. Kenton Musgrave 
 * 
 * Parameters:
 *    ``H''  determines the highest fractal dimension
 *    ``lacunarity''  is gap between successive frequencies
 *    ``octaves''  is the number of frequencies in the fBm
 *    ``offset''  is the zero offset, which determines multifractality
 */
float Fractal::MusgraveMultifractal(const Noise& n, Vector3 point, float H, float lacunarity, float octaves, float offset)
{
    float value, frequency, remainder;
    int i;
    static bool first = true;
    static float *exponent_array;

    /* precompute and store spectral weights */
    if (first) 
    {
        /* seize required memory for exponent_array */
        exponent_array = (float *)malloc( (octaves+1) * sizeof(float) );
        frequency = 1.0;
        for (i=0; i<=octaves; i++)
        {
            /* compute weight for each frequency */
            exponent_array[i] = pow( frequency, -H );
            frequency *= lacunarity;
        }
        first = false;
    }

    value = 1.0;
    frequency = 1.0;

    /* inner loop of multifractal construction */
    for (i=0; i<octaves; i++)
    {
        value *= offset * frequency * n.GetValue(point);
        point.x *= lacunarity;
        point.y *= lacunarity;
        point.z *= lacunarity;
    }

    remainder = octaves - (int)octaves;
    if (remainder)
        value += remainder * n.GetValue( point ) * exponent_array[i];

    return value;

}

// /* "Variable Lacunarity Noise"  -or- VLNoise3()
// * A distorted variety of Perlin noise.
// *
// * Copyright 1994 F. Kenton Musgrave 
// */
// float Fractal::VLNoise3(const Noise& n, const Vector3 point, float distortion )
// {
//     Vector offset, VecNoise3(), AddVectors();

//     offset.x = point.x +0.5;        /* misregister domain */
//     offset.y = point.y +0.5;
//     offset.z = point.z +0.5;

//     offset = VecNoise3( offset );   /* get a random vector */

//     offset.x *= distortion;         /* scale the randomization */
//     offset.y *= distortion;
//     offset.z *= distortion;

//     /* ``point'' is the domain; distort domain by adding ``offset'' */
//     point = AddVectors( point, offset );

//     return Noise3( point );         /* distorted-domain noise */
// }

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
    float value, increment, frequency, remainder;
    int i;
    static float first = true;
    static float *exponent_array;

    /* precompute and store spectral weights, for efficiency */
    if ( first ) {
        /* seize required memory for exponent_array */
        exponent_array = (float *)malloc( (octaves+1) * sizeof(float) );
        frequency = 1.0;
        for (i=0; i<=octaves; i++) 
        {
            /* compute weight for each frequency */
            exponent_array[i] = pow( frequency, -H );
            frequency *= lacunarity;
        }
        first = false;
    }

    /* first unscaled octave of function; later octaves are scaled */
    value = offset + n.GetValue(point);
    point.x *= lacunarity;
    point.y *= lacunarity;
    point.z *= lacunarity;

    /* spectral construction inner loop, where the fractal is built */
    for (i=1; i<octaves; i++) 
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
    remainder = octaves - (int)octaves;
    if ( remainder ) {
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
float Fractal::MusgraveHybridMultifractal(const Noise& n, Vector3 point, float H, float lacunarity, float octaves, float offset )
{
    float frequency, result, signal, weight, remainder; 
    int i;
    static bool first = true;
    static float *exponent_array;

    /* precompute and store spectral weights */
    if ( first ) {
        /* seize required memory for exponent_array */
        exponent_array = (float *)malloc( (octaves+1) * sizeof(float) );
        frequency = 1.0;
        for (i=0; i<=octaves; i++) 
        {
            /* compute weight for each frequency */
            exponent_array[i] = pow( frequency, -H);
            frequency *= lacunarity;
        }
        first = false;
    }

    /* get first octave of function */
    result = ( n.GetValue(point) + offset ) * exponent_array[0];
    weight = result;
    /* increase frequency */
    point.x *= lacunarity;
    point.y *= lacunarity;
    point.z *= lacunarity;

    /* spectral construction inner loop, where the fractal is built */
    for (i=1; i<octaves; i++) {
        /* prevent divergence */
        if ( weight > 1.0 )  weight = 1.0;

        /* get next higher frequency */
        signal = ( n.GetValue(point) + offset ) * exponent_array[i];
        /* add it in, weighted by previous freq's local value */
        result += weight * signal;
        /* update the (monotonically decreasing) weighting value */
        /* (this is why H must specify a high fractal dimension) */
        weight *= signal;

        /* increase frequency */
        point.x *= lacunarity;
        point.y *= lacunarity;
        point.z *= lacunarity;
    } /* for */

    /* take care of remainder in ``octaves''  */
    remainder = octaves - (int)octaves;
    if ( remainder )
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
    float result, frequency, signal, weight;
    int i;
    static bool first = true;
    static float *exponent_array;

    /* precompute and store spectral weights */
    if (first) 
    {
        /* seize required memory for exponent_array */
        exponent_array = (float *)malloc( (octaves+1) * sizeof(float) );
        frequency = 1.0;
        for (i=0; i<=octaves; i++) 
        {
            /* compute weight for each frequency */
            exponent_array[i] = pow( frequency, -H );
            frequency *= lacunarity;
        }
        first = false;
    }

    /* get first octave */
    signal = n.GetValue( point );
    /* get absolute value of signal (this creates the ridges) */
    if ( signal < 0.0 ) signal = -signal;
    /* invert and translate (note that "offset" should be ~= 1.0) */
    signal = offset - signal;
    /* square the signal, to increase "sharpness" of ridges */
    signal *= signal;
    /* assign initial values */
    result = signal;
    weight = 1.0;

    for( i=1; i<octaves; i++ ) 
    {
        /* increase the frequency */
        point.x *= lacunarity;
        point.y *= lacunarity;
        point.z *= lacunarity;

        /* weight successive contributions by previous signal */
        weight = signal * gain;
        if ( weight > 1.0 ) weight = 1.0;
        if ( weight < 0.0 ) weight = 0.0;
        signal = n.GetValue( point );
        if ( signal < 0.0 ) signal = -signal;
        signal = offset - signal;
        signal *= signal;
        /* weight the contribution */
        signal *= weight;
        result += signal * exponent_array[i];
    }

    return result;
}
