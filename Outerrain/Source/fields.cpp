#include "fields.h"
#include "vec.h"
#include <algorithm>

/* Scalarfield 2D */
Scalarfield2D::Scalarfield2D(int nx, int ny, Vector2 a, Vector2 b) : nx(nx), ny(ny), a(a), b(b)
{
	values.resize(nx * ny);
	std::fill(values.begin(), values.end(), 0);
}

int Scalarfield2D::Index(int i, int j) const
{
	return i * (nx - 1) + j;
}

double Scalarfield2D::At(int i, int j) const
{
	int index = Index(i, j);
	return values[index];
}

double Scalarfield2D::GetValueBilinear(const Vector2& p) const
{
	Vector2 q = p - a;
	Vector2 d = b - a;

	double u = q[0] / d[0];
	double v = q[1] / d[1];

	int i = int(u * (nx - 1));
	int j = int(v * (ny - 1));

	double v1 = At(i, j);
	double v2 = At(i + 1, j);
	double v3 = At(i + 1, j + 1);
	double v4 = At(i, j + 1);

	return (1 - u) * (1 - v) * v1 
			+ (1 - u) * v * v2 
			+ u * (1 - v) * v3 
			+ u * v * v4;
}


/* Heightfield */
Heightfield::Heightfield(int nx, int ny, Vector2 a, Vector2 b) : Scalarfield2D(nx, ny, a, b)
{
}

double Heightfield::Height(const Vector2& p) const
{
	return GetValueBilinear(p);
}