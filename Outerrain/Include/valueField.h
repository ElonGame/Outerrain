#pragma once

#include <vector>
#include <array>
#include <algorithm>
#include "vec.h"
#include "image.h"

/* Utiity struct */
typedef struct Point
{
	int x, y;
	float value;
	Point() { }
	Point(int a, int b, float h) : x(a), y(b), value(h) { }
} Point;

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

		float u = q[0] / d[0];
		float v = q[1] / d[1];

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

		float texelX = 1.0f / (static_cast<float>(nx - 1));
		float texelY = 1.0f / (static_cast<float>(ny - 1));

		float u = q[0] / d[0];
		float v = q[1] / d[1];

		int i = int(v * (ny - 1));
		int j = int(u * (nx - 1));

		float anchorU = j * texelX;
		float anchorV = i * texelY;

		float localU = (u - anchorU) / texelX;
		float localV = (v - anchorV) / texelY;

		T v1 = Get(i, j);
		T v2 = Get(i + 1, j);
		T v3 = Get(i + 1, j + 1);
		T v4 = Get(i, j + 1);

		return (1 - localU) * (1 - localV) * v1
			+ (1 - localU) * localV * v2
			+ localU * (1 - localV) * v4
			+ localU * localV * v3;
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


class ScalarField2D : public ValueField<float>
{
public:
	ScalarField2D() : ValueField()
	{
	}

	ScalarField2D(int nx, int ny, Vector2 bottomLeft, Vector2 topRight)
		: ValueField(nx, ny, bottomLeft, topRight)
	{
	}

	ScalarField2D(int nx, int ny, Vector2 bottomLeft, Vector2 topRight, float value)
		: ValueField(nx, ny, bottomLeft, topRight, value)
	{
	}

	void WriteImageGrayscale(const char* path)
	{
		Image im = Image(nx, ny);
		float min = MinValue();
		float max = MaxValue();
		for (int i = 0; i < ny; i++)
		{
			for (int j = 0; j < nx; j++)
			{
				float v = (Get(i, j) - min) / (max - min);
				im(i, j) = Color(v, v, v, 1.0);
			}
		}
		im.WriteImage(path);
	}

	void ReadImageGrayscale(const char* file, int blackAltitude, int whiteAltitude)
	{
		Image heightmap;
		heightmap.ReadImage(file);
		float texelX = 1.0f / (heightmap.Width());
		float texelY = 1.0f / (heightmap.Height());

		for (int i = 0; i < ny; i++)
		{
			for (int j = 0; j < nx; j++)
			{
				float u = j / (static_cast<float>(nx - 1));
				float v = i / (static_cast<float>(ny - 1));

				int anchorX = static_cast<int>((u * (heightmap.Width() - 1)));
				int anchorY = static_cast<int>((v * (heightmap.Height() - 1)));
				if (anchorX == heightmap.Width() - 1)
					anchorX--;
				if (anchorY == heightmap.Height() - 1)
					anchorY--;

				float a = heightmap(anchorX, anchorY).r;
				float b = heightmap(anchorX, anchorY + 1).r;
				float c = heightmap(anchorX + 1, anchorY + 1).r;
				float d = heightmap(anchorX + 1, anchorY).r;

				float anchorU = anchorX * texelX;
				float anchorV = anchorY * texelY;

				float localU = (u - anchorU) / texelX;
				float localV = (v - anchorV) / texelY;

				float abu = Lerp(a, b, localU);
				float dcu = Lerp(d, c, localU);

				float value = Lerp(dcu, abu, localV);
				Set(i, j, blackAltitude + value * (whiteAltitude - blackAltitude));
			}
		}
	}

	Vector2 Gradient(int i, int j) const
	{
		Vector2 ret;
		float cellSize = abs(TopRight().x - BottomLeft().x) / SizeX();

		// X Gradient
		if (i == 0)
			ret.x = (Get(i + 1, j) - Get(i, j)) / cellSize;
		else if (i == ny - 1)
			ret.x = (Get(i, j) - Get(i - 1, j)) / cellSize;
		else
			ret.x = (Get(i + 1, j) - Get(i - 1, j)) / (2.0f * cellSize);

		// Y Gradient
		if (j == 0)
			ret.y = (Get(i, j + 1) - Get(i, j)) / cellSize;
		else if (j == nx - 1)
			ret.y = (Get(i, j) - Get(i, j - 1)) / cellSize;
		else
			ret.y = (Get(i, j + 1) - Get(i, j - 1)) / (2.0f * cellSize);

		return ret;
	}

	void Fill(float v)
	{
		std::fill(values.begin(), values.end(), v);
	}

	float MaxValue() const
	{
		return *std::max_element(values.begin(), values.end());
	}

	float MinValue() const
	{
		return *std::min_element(values.begin(), values.end());
	}

	float AverageValue() const
	{
		float ret = 0.0f;
		for (int i = 0; i < values.size(); i++)
			ret += values[i];
		return ret / static_cast<float>(values.size());
	}
};