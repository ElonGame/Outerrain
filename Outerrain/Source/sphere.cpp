#include "sphere.h"

Sphere::Sphere(const Vector3& c, float r) : center(c), radius(r)
{

}

void Sphere::Translate(const Vector3& T)
{
	center += T;
}

float Sphere::Distance(const Vector3& p) const
{
	float d = Magnitude(p - center);
	return d < radius ? 0 : d;
}

bool Sphere::Contains(const Vector3& p) const
{
	return Distance(p) == 0;
}

Vector3 Sphere::Center() const
{
	return center;
}

float Sphere::Radius() const
{
	return radius;
}

std::ostream& operator<<(std::ostream& stream, const Sphere& s)
{
	stream << s.center << " ; " << s.radius;
	return stream;
}