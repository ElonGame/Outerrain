#pragma once
#include <vector>
#include "vec.h"
#include "mesh.h"
#include "valueField.h"


struct Point
{
	int x, y;

	Point() { }
	Point(int a, int b) : x(a), y(b) { }
};


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
	Vector3 Normal(int, int) const;
	Vector3 Vertex(int, int) const;
	double Height(const Vector2&) const;
	double NormalizedHeight(const Vector2&) const;
	Mesh* GetMesh() const;
	void SetHeight(int, int, double);
	int SizeX() const { return nx; }
	int SizeY() const { return ny; }
	void ComputeNormalField();

	void Erode(int, double);
	void Transport(int, double);

	int Distribute(Point p, Point* neighbours, float* height, float* slope) const;
	ScalarField2D Drainage() const;
	ScalarField2D DrainageSqrt() const;
	double ComputeIllumination(int i, int j) const;

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
	void StreamPowerErosion(int);

	int SizeX() const { return nx; }
	int SizeY() const { return ny; }
	double Height(int, int) const;
	double SandValue(int, int) const;
	double BeckrockValue(int, int) const;
	Mesh* GetMesh() const;
};


/* Vegetation Terrain */
class VegetationTerrain : public Terrain2D
{
protected:
	ScalarField2D vegetationDensityField;

public:
	VegetationTerrain() { }
	VegetationTerrain(int, int, Vector2, Vector2);

	void ComputeDensities();

	std::vector<GameObject*> GetTreeObjects() const;
	std::vector<Vector2> VegetationTerrain::GetRandomDistribution(float objRadius, float tileSize, int maxTries) const;
};