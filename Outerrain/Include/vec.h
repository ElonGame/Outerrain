#pragma once
#include <math.h>
#include <iostream>


/* Forward Declarations */
struct Vector2;
struct Vector3;
struct Vector4;


/* Vector2 */
struct Vector2
{
public:
	double x, y;

	Vector2() : x(0.0), y(0.0) {}
	Vector2(double n) : x(n), y(n) {}
	Vector2(double x, double y) : x(x), y(y) {}

	bool operator==(const Vector2& u) const
	{
		return (x == u.x && y == u.y);
	}
	Vector2 operator-(const Vector2& u) const
	{
		return Vector2(x - u.x, y - u.y);
	}
	Vector2 operator+(const Vector2& u) const
	{
		return Vector2(x + u.x, y + u.y);
	}
	double operator[](int i)
	{
		if (i == 0)
			return x;
		return y;
	}
	friend std::ostream& operator<< (std::ostream& stream, const Vector2& u);
};
inline std::ostream& operator<<(std::ostream& stream, const Vector2& u)
{
	stream << "(" << u.x << ", " << u.y << ");";
	return stream;
}
inline double Dot(const Vector2& u, const Vector2& v)
{
	return u.x * v.x + u.y * v.y;
}
inline double Magnitude(const Vector2& u)
{
	return sqrt(u.x * u.x + u.y * u.y);
}


/* Vector3 */
struct Vector3
{
public:
	double x, y, z;

	explicit Vector3() : x(0.0), y(0.0), z(0.0) { }
	explicit Vector3(double n) : x(n), y(n), z(n) {}
	explicit Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

	bool operator==(const Vector3& u) const
	{
		return (x == u.x && y == u.y && z == u.z);
	}
	Vector3 operator-(const Vector3& u) const
	{
		return Vector3(x - u.x, y - u.y, z - u.z);
	}
	Vector3 operator+(const Vector3& u) const
	{
		return Vector3(x + u.x, y + u.y, z + u.z);
	}
	double operator[](int i)
	{
		if (i == 0)
			return x;
		else if (i == 1)
			return y;
		return z;
	}
	friend std::ostream& operator<<(std::ostream& stream, const Vector3& u);
};
inline std::ostream& operator<<(std::ostream& stream, const Vector3& u)
{
	stream << "(" << u.x << ", " << u.y << ", " << u.z << ");";
	return stream;
}
inline Vector3 Cross(const Vector3& u, const Vector3& v)
{
	return Vector3(
		(u.y * v.z) - (u.z * v.y),
		(u.z * v.x) - (u.x * v.z),
		(u.x * v.y) - (u.y * v.x));
}
inline double Dot(const Vector3& u, const Vector3& v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}
inline double Magnitude(const Vector3& u)
{
	return sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
}


/* Vector4 */
struct Vector4
{
public:
	double x, y, z, w;

	Vector4() : x(0.0), y(0.0), z(0.0), w(0.0) {}
	Vector4(double n) : x(n), y(n), z(n), w(n) {}
	Vector4(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

	bool operator==(const Vector4& u) const
	{
		return (x == u.x && y == u.y && z == u.z && w == u.w);
	}
	Vector4 operator-(const Vector4& u) const
	{
		return Vector4(x - u.x, y - u.y, z - u.z, w - u.w);
	}
	Vector4 operator+(const Vector4& u) const
	{
		return Vector4(x + u.x, y + u.y, z + u.z, w + u.w);
	}
	double operator[](int i)
	{
		if (i == 0)
			return x;
		else if (i == 1)
			return y;
		else if (i == 2)
			return z;
		return w;
	}
	friend std::ostream& operator<<(std::ostream& stream, const Vector4& u);
};
inline std::ostream& operator<<(std::ostream& stream, const Vector4& u)
{
	stream << "(" << u.x << ", " << u.y << ", " << u.z << ", " << u.w << ");";
	return stream;
}
