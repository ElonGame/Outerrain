#pragma once
#include <vector>
#include "vec.h"
#include "mesh.h"
#include "valueField.h"

/* Terrain2D */
class Terrain2D
{
protected:
	int nx, ny;
	Vector2 bottomLeft, topRight;
	ScalarField2D heightField;
	ValueField<Vector3> normalField;

public:
	Terrain2D() { }
	Terrain2D(int, int, Vector2, Vector2);

	void InitFromNoise(int, int);
	void InitFromFile(const char*, int, int);
	int Distribute(Point p, std::array<Point, 8>& neighbours, std::array<float, 8>& height, std::array<float, 8>& slope) const;

	Vector3 Normal(int, int) const;
	Vector3 Vertex(int, int) const;
	float Height(const Vector2&) const;
	float NormalizedHeight(const Vector2&) const;
	Mesh* GetMesh() const;
	void SetHeight(int, int, float);
	int SizeX() const { return nx; }
	int SizeY() const { return ny; }
	std::vector<Vector3> GetAllVertices() const;

	/* Erosion */
	void StreamPowerErosion(int stepCount, float);
	void ThermalErosion(int stepCount);

	/* Useful fields */
	void ComputeNormalField();
	ScalarField2D DrainageField() const;
	ScalarField2D DrainageSqrtField() const;
	ScalarField2D WetnessField() const;
	ScalarField2D StreamPowerField() const;
	ScalarField2D SlopeField() const;
	ScalarField2D AccessibilityField() const;
};


/* LayerTerrain2D */
class LayerTerrain2D
{
protected:
	int nx, ny;
	Vector2 a, b;
	ScalarField2D bedrock;
	ScalarField2D sand;

public:
	LayerTerrain2D() { }
	LayerTerrain2D(int, int, Vector2, Vector2);

	void InitFromFile(const char*, int, int, float);
	void ThermalErosion(int);

	int SizeX() const { return nx; }
	int SizeY() const { return ny; }
	Vector3 Vertex(int, int) const;
	float Height(int, int) const;
	float SandValue(int, int) const;
	float BeckrockValue(int, int) const;
	Mesh* GetMesh() const;
	std::vector<Vector3> GetAllVertices() const;
};


/* Vegetation Terrain */
class VegetationTerrain : public Terrain2D
{
protected:
	ScalarField2D vegetationDensityField;

public:
	VegetationTerrain() { }
	VegetationTerrain(int, int, Vector2, Vector2);

	ScalarField2D VegetationDensityField() const;

	void ComputeVegetationDensities();

	std::vector<GameObject*> GetTreeObjects() const;
	std::vector<std::vector<Vector2>> GetRotatedRandomDistributions(float objRadius, float tileSize, int maxTries) const;
};