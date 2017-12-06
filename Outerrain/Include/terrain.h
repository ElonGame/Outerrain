#pragma once
#include <vector>
#include "vec.h"
#include "mesh.h"
#include "valueField.h"

class Terrain2D
{
protected:
	int nx, ny;
	Vector2 bottomLeft, topRight;
	ValueField<double> heightField;
	ValueField<Vector3> normalField;

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
	ValueField<double> bedrock;
	ValueField<double> sand;

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
