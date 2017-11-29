#pragma once
#include <vector>

class Scalarfield2D
{
protected:
	int nx, ny;
	std::vector<double> values;

public:
	Scalarfield2D() { }
	Scalarfield2D(int, int);

	// Todo
	// int Index(int, int) => retourne index1D à partir de index2D
	// double At(int, int) => return value sans interpolation
	// GetValue(double, double) => return value avec interpolation
	int Index(int, int) const;
	double At(int, int) const;
	double GetValue(double, double) const;
};
