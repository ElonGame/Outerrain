#pragma once
#include <vector>
#include "vec.h"
#include "mesh.h"

class Scalarfield2D
{
protected:
	int nx, ny;
	Vector2 bottomLeft, topRight;
	std::vector<double> values;

public:
	Scalarfield2D() { }
	Scalarfield2D(int, int, Vector2, Vector2);

	int Index(int, int) const;
	double Get(int, int) const;
	void Set(int, int, double);
	double GetValueBilinear(const Vector2&) const;
	double SizeX() const { return nx; }
	double SizeY() const { return ny; }
};

class Vector3field2D
{
protected:
	int nx, ny;
	Vector2 bottomLeft, topRight;
	std::vector<Vector3> values;

public:
	Vector3field2D() { }
	Vector3field2D(int, int, Vector2, Vector2);
	int Index(int, int) const;
	Vector3 Get(int, int) const;
	void Set(int, int, Vector3);
	Vector3 GetValueBilinear(const Vector2&) const;
};

class Terrain2D
{
protected:
	int nx, ny;
	Vector2 bottomLeft, topRight;
	Scalarfield2D heightField;
	Vector3field2D normalField;

public:
	Terrain2D() { }
	Terrain2D(int, int, Vector2, Vector2);
	void InitFromFile(const char* file, float blackAltitude, float whiteAltitude);
	void InitFromNoise(int, int);
	Vector3 Normal(int, int) const;
	Vector3 Vertex(int, int) const;
	double Height(const Vector2&) const;
	Mesh GetMesh() const;
	void SetHeight(int, int, double);

	int SizeX() const { return nx; }
	int SizeY() const { return ny; }

private:
	float Lerp(float a, float b, float f);
	void ComputeNormalFieldFromHeightField();
};


class LayerTerrain2D
{
protected:
	int nx, ny;
	Vector2 a, b;
	Scalarfield2D bedrock;
	Scalarfield2D sand;

public:
	LayerTerrain2D() { }
	LayerTerrain2D(int, int, Vector2, Vector2);

	int SizeX() const { return nx; }
	int SizeY() const { return ny; }
	double Height(int, int) const;
	double SandValue(int, int) const;
	double BeckrockValue(int, int) const;
	void ThermalErosion(int);
	Mesh GetMesh() const;
};
