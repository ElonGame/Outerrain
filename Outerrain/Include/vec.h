#pragma once
#include <math.h>
#include <iostream>


/* Vector2 */
class Vector2
{
private:
	double x, y;

public:
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

	const double& X() const { return x; }
	const double& Y() const { return y; }

	void SetX(double x) { this->x = x; }
	void SetY(double y) { this->y = y; }
};
inline std::ostream& operator<<(std::ostream& stream, const Vector2& u)
{
	stream << "(" << u.x << ", " << u.y << ");";
	return stream;
}
inline double Dot(const Vector2& u, const Vector2& v)
{
	return u.X() * v.X() + u.Y() * v.Y();
}
inline double Magnitude(const Vector2& u)
{
	return sqrt(u.X() * u.X() + u.Y() * u.Y());
}


/* Vector3 */
class Vector3
{
private:
	double x, y, z;

public:
	Vector3() : x(0.0), y(0.0), z(0.0) {}
	Vector3(double n) : x(n), y(n), z(n) {}
	Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

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

	const double& X() const { return x; }
	const double& Y() const { return y; }
	const double& Z() const { return z; }

	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	void setZ(double z) { this->z = z; }
};
inline std::ostream& operator<<(std::ostream& stream, const Vector3& u)
{
	stream << "(" << u.x << ", " << u.y << ", " << u.z << ");";
	return stream;
}
inline Vector3 Cross(const Vector3& u, const Vector3& v)
{
	return Vector3(
		(u.Y() * v.Z()) - (u.Z() * v.Y()),
		(u.Z() * v.X()) - (u.X() * v.Z()),
		(u.X() * v.Y()) - (u.Y() * v.X()));
}
inline double Dot(const Vector3& u, const Vector3& v)
{
	return u.X() * v.X() + u.Y() * v.Y() + u.Z() * v.Z();
}
inline double Magnitude(const Vector3& u)
{
	return sqrt(u.X() * u.X() + u.Y() * u.Y() + u.Z() * u.Z());
}


/* Vector4 */
class Vector4
{
private:
	double x, y, z, w;

public:
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

	const double& X() const { return x; }
	const double& Y() const { return y; }
	const double& Z() const { return z; }
	const double& W() const { return w; }

	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	void setZ(double z) { this->z = z; }
	void setW(double w) { this->w = w; }
};
inline std::ostream& operator<<(std::ostream& stream, const Vector4& u)
{
	stream << "(" << u.x << ", " << u.y << ", " << u.z << ", " << u.w << ");";
	return stream;
}