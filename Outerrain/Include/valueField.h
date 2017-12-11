#pragma once

#include <vector>
#include <algorithm>
#include "vec.h"

template<typename T>
class ValueField 
{
protected:
	int nx, ny;
	Vector2 bottomLeft, topRight;
	std::vector<T> values;

public:
	ValueField()
		: nx(0), ny(0), bottomLeft(Vector2(0)), topRight(Vector2(0)) 
	{ 
	}

	ValueField(int nx, int ny, Vector2 bottomLeft, Vector2 topRight)
		: nx(nx), ny(ny), bottomLeft(bottomLeft), topRight(topRight)
	{
		values.resize(nx * ny, T(0));
	}

	ValueField(int nx, int ny, Vector2 bottomLeft, Vector2 topRight, T value)
		: nx(nx), ny(ny), bottomLeft(bottomLeft), topRight(topRight)
	{
		values.resize(nx * ny, value);
	}

	int Index(int row, int column) const
	{
		return row * nx + column;
	}
	
	T Get(int row, int column) const
	{
		int index = Index(row, column);
		return values[index];
	}

	bool IsInsideField(const Vector2& p) const
	{
		Vector2 q = p - bottomLeft;
		Vector2 d = topRight - bottomLeft;

		double u = q[0] / d[0];
		double v = q[1] / d[1];

		int j = int(u * (nx - 1));
		int i = int(v * (ny - 1));

		if (i < 0 || i >= ny - 1 || j < 0 || j >= nx - 1)
			return false;
		return true;
	}

	T GetValueBilinear(const Vector2& p) const
	{
		Vector2 q = p - bottomLeft;
		Vector2 d = topRight - bottomLeft;

		double u = q[0] / d[0];
		double v = q[1] / d[1];

		int i = int(u * (nx - 1));
		int j = int(v * (ny - 1));

		T v1 = Get(i, j);
		T v2 = Get(i + 1, j);
		T v3 = Get(i + 1, j + 1);
		T v4 = Get(i, j + 1);

		return (1 - u) * (1 - v) * v1
				+ (1 - u) * v * v2
				+ u * (1 - v) * v3
				+ u * v * v4;
	}
	
	void Set(int row, int column, T v) 
	{
		values[Index(row, column)] = v;
	}
	
	double SizeX() const 
	{ 
		return nx; 
	}
	
	double SizeY() const 
	{ 
		return ny; 
	}
};

class ScalarField2D : public ValueField<double> 
{
public:
	ScalarField2D() : ValueField() 
	{ 
	}

	ScalarField2D(int nx, int ny, Vector2 bottomLeft, Vector2 topRight)
		: ValueField(nx, ny, bottomLeft, topRight)
	{
	}

	ScalarField2D(int nx, int ny, Vector2 bottomLeft, Vector2 topRight, double value)
		: ValueField(nx, ny, bottomLeft, topRight, value)
	{
	}

	// @Todo Nathan
	void WriteImage(const char* path)
	{
		// @Todo Nathan
		// Ecrit les valeurs de double du scalarfield dans une Image gkit
		// pour visualiser en niveau de gris.
		// Utile pour voir toutes les maps etc...
	}

	double MaxValue() const
	{
		return *std::max_element(values.begin(), values.end());
	}
};