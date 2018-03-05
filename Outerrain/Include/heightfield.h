#pragma once

#include "scalarfield.h"
#include "ray.h"
#include "meshModel.h"
#include "fractal.h"

class Heightfield : public Scalarfield2D
{
public:
	Heightfield();
	Heightfield(int nx, int ny, const Box2D& bbox);
	Heightfield(int nx, int ny, const Box2D& bbox, float value);
	Heightfield(const std::string& filePath, float minAltitude, float maxAltitude, int nx, int ny, const Box2D& bbox);
	Heightfield(int nx, int ny, const Box2D& bbox, const Noise& n, float amplitude, float freq, int oct, FractalType type);
	Heightfield(int nx, int ny, const Box2D& bbox, const Noise& n, float amplitude, float freq, int oct, const Vector3& offset, FractalType type);
	~Heightfield();

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

	std::vector<Vector3> GetAllNormals() const;
	MeshModel* GetMeshModel() const;
};
