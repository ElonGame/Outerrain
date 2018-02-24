#pragma once

#include "scalarfield.h"
#include "ray.h"
#include "meshModel.h"
#include "fractal.h"

class Heightfield : public Scalarfield2D
{
public:
	Heightfield();
	Heightfield(int nx, int ny, const Vector2& bottomLeft, const Vector2& topRight);
	Heightfield(int nx, int ny, const Vector2& bottomLeft, const Vector2& topRight, float value);
	Heightfield(const std::string& filePath, int minAltitude, int maxAltitude, int nx, int ny, const Vector2& bottomLeft, const Vector2& topRight);
	Heightfield(int nx, int ny, const Vector2& bottomLeft, const Vector2& topRight, const Noise& n, float amplitude, float freq, int oct, FractalType type);
	~Heightfield() { }

	void ThermalWeathering(int stepCount, float amplitude);
	void StreamPowerErosion(int stepCount, float amplitude);

	Scalarfield2D DrainageArea() const;
	Scalarfield2D Wetness() const;
	Scalarfield2D StreamPower() const;
	Scalarfield2D Slope() const;
	Scalarfield2D Illumination() const;

	bool Intersect(const Ray& ray, Hit& hit, float K) const;
	bool Intersect(const Ray& ray, Hit& hit) const;
	bool Intersect(const Vector3& origin, const Vector3& direction, Vector3& hitPos, Vector3& hitNormal) const;

	MeshModel* GetMeshModel() const;
};
