#include "fields.h"
#include <algorithm>
using namespace std;

Scalarfield2D::Scalarfield2D(int nx, int ny) : nx(nx), ny(ny)
{
	values.resize(nx * ny);
	std::fill(values.begin(), values.end(), 0);
}

int Scalarfield2D::Index(int i, int j) const
{
	return i * nx + j;
}

double Scalarfield2D::At(int i, int j) const
{
	int index = Index(i, j);
	return values[index];
}

double Scalarfield2D::GetValue(double u, double v) const
{
	return 0.0;
}