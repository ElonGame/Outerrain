#include "vec.h"

namespace Math
{
    inline float Lerp(float a, float b, float f)
    {
        return (a * (1.0f - f)) + (b * f);
    }

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

    inline float Min(float a, float b)
    {
        return a < b ? a : b;
    }

    inline float Max(float a, float b)
    {
        return a > b ? a : b;
    }

    inline Vector3 Center(const Vector3& a, const Vector3& b)
    {
        return (a + b) / 2;
    }

    inline Vector2 Center(const Vector2& a, const Vector2& b)
    {
	    return (a + b) / 2;
    }
}