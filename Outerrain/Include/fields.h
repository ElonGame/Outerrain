#pragma once
#include <vector>
#include "vec.h"
#include "mesh.h"

class Scalarfield2D
{
protected:
	int nx, ny;
	Vector2 a, b;
	std::vector<double> values;

public:
	Scalarfield2D() { }
	Scalarfield2D(int, int, Vector2, Vector2);

	int Index(int, int) const;
	double At(int, int) const;
	double GetValueBilinear(const Vector2&) const;
};

class Heightfield : public Scalarfield2D
{
public:
	Heightfield() : Scalarfield2D() { }
	Heightfield(int, int, Vector2, Vector2);

	// @Todo
	void InitFromFile();
	void InitFromNoise();

	Vector3 Normal(int, int) const;
	Vector3 Vertex(int, int) const;
	double Height(const Vector2&) const;
	Mesh GetMesh() const;
};
