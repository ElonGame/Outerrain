#pragma once

#include <vector>
#include <array>
#include <algorithm>

#include "vec.h"
#include "box2D.h"
#include "image.h"

/* Utility struct */
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
	Box2D box;
	std::vector<T> values;

public:
	ValueField() : nx(0), ny(0), box(Vector2(0), Vector2(0))
	{
	}

	ValueField(int nx, int ny, const Box2D& bbox) : nx(nx), ny(ny), box(bbox)
	{
		values.resize(nx * ny, T(0));
	}

	ValueField(int nx, int ny, const Box2D& bbox, const T& value) : nx(nx), ny(ny), box(bbox)
	{
		values.resize(nx * ny, value);
	}

	virtual ~ValueField() { }

	bool Inside(const Vector2& p) const
	{
		Vector2 q = p - box.Vertex(0);
		Vector2 d = box.Vertex(1) - box.Vertex(0);

		float u = q[0] / d[0];
		float v = q[1] / d[1];

		int j = int(u * (nx - 1));
		int i = int(v * (ny - 1));

		if (i < 0 || i >= ny - 1 || j < 0 || j >= nx - 1)
			return false;
		return true;
	}

	bool Inside(int i, int j) const
	{
		if (i < 0 || i >= nx || j < 0 || j >= ny)
			return false;
		return true;
	}

	bool Inside(const Vector2i& v) const
	{
		if (v.x < 0 || v.x >= nx || v.y < 0 || v.y >= ny)
			return false;
		return true;
	}


	void ToIndex2D(int index, int& i, int& j) const
	{
		i = index / nx;
		j = index % nx;
	}

	Vector2i ToIndex2D(int index) const
	{
		return Vector2i(index / nx, index % nx);
	}

	int ToIndex1D(const Vector2i& v) const
	{
		return v.x * nx + v.y;
	}

	int ToIndex1D(int i, int j) const
	{
		return i * nx + j;
	}


	T Get(int row, int column) const
	{
		int index = ToIndex1D(row, column);
		return values[index];
	}

	T Get(int index) const
	{
		return values[index];
	}

	T Get(const Vector2i& v) const
	{
		int index = ToIndex1D(v);
		return values[index];
	}

	T GetValueBilinear(const Vector2& p) const
	{
		Vector2 q = p - box.Vertex(0);
		Vector2 d = box.Vertex(1) - box.Vertex(0);

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


	void Fill(T v)
	{
		std::fill(values.begin(), values.end(), v);
	}

	void Set(int row, int column, T v)
	{
		values[ToIndex1D(row, column)] = v;
	}

	void Set(const Vector2i& coord, T v)
	{
		values[ToIndex1D(coord)] = v;
	}

	void Set(int index, T v)
	{
		values[index] = v;
	}


	T Max() const
	{
		return *std::max_element(values.begin(), values.end());
	}

	T Min() const
	{
		return *std::min_element(values.begin(), values.end());
	}


	Vector2i Size() const
	{
		return Vector2i(nx, ny);
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
		return box.Vertex(0);
	}

	Vector2 TopRight() const
	{
		return box.Vertex(1);
	}

	Box2D GetBox() const
	{
		return box;
	}
};
