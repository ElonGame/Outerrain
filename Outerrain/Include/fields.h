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
	double At(int, int) const;
	double GetValueBilinear(const Vector2&) const;
	double SizeX() const { return nx; }
	double SizeY() const { return ny; }
};


class Heightfield : public Scalarfield2D
{
public:
	Heightfield() : Scalarfield2D() { }
	Heightfield(int, int, Vector2, Vector2);

	void InitFromFile(const char* file, float blackAltitude, float whiteAltitude);
	void InitFromNoise(int, int);

	Vector3 Normal(int, int) const;
	Vector3 Vertex(int, int) const;
	double Height(const Vector2&) const;
	Mesh GetMesh() const;
	bool Inside(const Vector3&) const; // TODO Nathan

private:
	float Lerp(float a, float b, float f);
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

	double Height(int, int);
	double SandValue(int, int);
	double BeckrockValue(int, int);
	void ThermalErosion(int);
	Mesh GetMesh() const;
};
