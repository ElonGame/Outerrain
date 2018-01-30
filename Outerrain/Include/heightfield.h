#pragma once

#include "scalarfield.h"
#include "ray.h"
#include "mesh.h"

class Heightfield : public Scalarfield2D
{
public:
	Heightfield();
	Heightfield(const int& nx, const int& ny, const Vector2& bottomLeft, const Vector2& topRight);
	Heightfield(const int& nx, const int& ny, const Vector2& bottomLeft, const Vector2& topRight, const float& value);
	Heightfield(const std::string& filePath, const int& minAltitude, const int& maxAltitude, const int& nx, const int& ny, const Vector2& bottomLeft, const Vector2& topRight);

	void Thermal(const int& stepCount, const float& E);
	void StreamPowerErosion(const int& stepCount, const float& E);

	Scalarfield2D DrainageArea() const;
	Scalarfield2D Wetness() const;
	Scalarfield2D StreamPower() const;
	Scalarfield2D Slope() const;
	Scalarfield2D Illumination() const;

	bool Intersect(const Ray& ray, Hit& hit, const float& K) const;
	bool Intersect(const Ray& ray, Hit& hit) const;
	bool Intersect(const Vector3& origin, const Vector3 direction, Vector3& hitPos, Vector3& hitNormal) const;

	Mesh* CreateMesh() const;
};
