#pragma once
#include "vec.h"

namespace Math
{
	inline float Clamp(float x, float a = 0.0f, float b = 1.0f)
	{
		return x < a ? a : x > b ? b : x;
	}

	template<typename T, size_t N>
	inline T Accumulate(std::array<T, N>& arr)
	{
		T ret(0);
		for (unsigned int i = 0; i < N; i++)
			ret += arr[i];
		return ret;
	}

	template<typename T>
	inline T ChangeBase(T min, T max, T newMin, T newMax, T val)
	{
		return (newMax - newMin) * (val - min) / (max - min);
	}

	template<typename T>
	inline T Min(T a, T b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	inline T Max(T a, T b)
	{
		return a > b ? a : b;
	}

	template<typename T>
    inline T Lerp(T a, T b, float t)
    {
        return (a * (1.0f - t)) + (b * t);
    }

	template<typename T>
    inline T Center(const T& a, const T& b)
    {
	    return (a + b) / 2;
    }
}