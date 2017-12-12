#pragma once

#include <vector>
#include <algorithm>
#include "vec.h"
#include "image.h"

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

	bool InsideVertex(int i, int j) const
	{
		if (i < 0 || i >= nx || j < 0 || j >= ny)
			return false;
		return true;
	}

	void Index2D(int index, int& i, int& j) const
	{
		i = index / nx;
		j = index % nx;
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

	T Get(int index) const
	{
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

	void Set(int index, T v)
	{
		values[index] = v;
	}

	int SizeX() const
	{
		return nx;
	}

	int SizeY() const
	{
		return ny;
	}

	Vector2 BottomLeft() const
	{
		return bottomLeft;
	}

	Vector2 TopRight() const 
	{ 
		return topRight;
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

	void WriteImageGrayscale(const char* path)
	{
		Image im = Image(nx, ny);
		double min = MinValue();
		double max = MaxValue();
		for (int i = 0; i < ny; i++)
		{
			for (int j = 0; j < nx; j++)
			{
				double v = (Get(i, j) - min) / (max - min);
				im(i, j) = Color(v, v, v, 1.0);
			}
		}
		im.WriteImage(path);
	}

	Vector2 Gradient(int i, int j) const
	{
		Vector2 ret;
		double d = nx - 1;

		// X Gradient
		if (i == 0)
			ret.x = (Get(i + 1, j) - Get(i, j)) / d;
		else if (i == ny - 1)
			ret.x = (Get(i, j) - Get(i - 1, j)) / d;
		else
			ret.x = (Get(i + 1, j) - Get(i - 1, j)) / (2.0 * d);

		// Y Gradient
		if (j == 0)
			ret.y = (Get(i, j + 1) - Get(i, j)) / d;
		else if (j == nx - 1)
			ret.y = (Get(i, j) - Get(i, j - 1)) / d;
		else
			ret.y = (Get(i, j + 1) - Get(i, j - 1)) / (2.0 * d);

		return ret;
	}

	int LowestNeighbor(int index) const
	{
		int i, j;
		Index2D(index, i, j);

		// Todo
		//InsideVertex();

		int indexLVoisin = -1;
		if (j < SizeY() - 1) {
			if (Get(i, j) > Get(i, j + 1))
				indexLVoisin = Index(i, j + 1);
		}
		if (i < SizeX() - 1) {
			if (Get(i, j) > Get(i + 1, j)) {
				if (indexLVoisin = -1 || Get(indexLVoisin) > Get(i + 1, j))
					indexLVoisin = Index(i + 1, j);
			}
		}
		if (i > 0) {
			if (Get(i, j) > Get(i - 1, j)) {
				if (indexLVoisin = -1 || Get(indexLVoisin) > Get(i - 1, j))
					indexLVoisin = Index(i - 1, j);
			}
		}
		if (j > 0) {
			if (Get(i, j) > Get(i, j - 1)) {
				if (indexLVoisin = -1 || Get(indexLVoisin) > Get(i, j - 1))
					indexLVoisin = Index(i, j - 1);
			}
		}
		return indexLVoisin;
	}

	double MaxValue() const
	{
		return *std::max_element(values.begin(), values.end());
	}

	double MinValue() const
	{
		return *std::min_element(values.begin(), values.end());
	}
};