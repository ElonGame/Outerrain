#pragma once

#include "scalarfield2D.h"
#include "ray.h"
#include "mesh.h"
#include "fractal.h"
#include "terrainSettings.h"
#include "frame.h"

class HeightField : public ScalarField2D
{
public:
	HeightField();
	HeightField(const TerrainSettings& settings);
	HeightField(int nx, int ny, const Box2D& bbox);
	HeightField(int nx, int ny, const Box2D& bbox, float value);
	HeightField(const std::string& filePath, float minAltitude, float maxAltitude, int nx, int ny, const Box2D& bbox);
	HeightField(int nx, int ny, const Box2D& bbox, const Noise& n, float amplitude, float freq, int oct, FractalType type);
	HeightField(int nx, int ny, const Box2D& bbox, const Noise& n, float amplitude, float freq, int oct, const Vector3& offset, FractalType type);
	~HeightField();

	void InitFromNoise(const Noise& n, float amplitude, float freq, int oct, const Vector3& offset, FractalType type);

	virtual void ThermalWeathering(float amplitude, float tanThresholdAngle = 0.6f);
	virtual void StreamPowerErosion(float amplitude);
	virtual void HydraulicErosion();

	ScalarField2D DrainageArea() const;
	ScalarField2D Wetness() const;
	ScalarField2D StreamPower() const;
	ScalarField2D Slope() const;
	ScalarField2D Illumination() const;

	bool Intersect(const Ray& ray, Hit& hit, float K) const;
	bool Intersect(const Ray& ray, Hit& hit) const;
	bool Intersect(const Vector3& origin, const Vector3& direction, Vector3& hitPos, Vector3& hitNormal) const;

	std::vector<Frame> GetVoxelFrames() const;
	Mesh* GetMesh() const;
};
