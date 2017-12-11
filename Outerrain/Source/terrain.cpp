#include <algorithm>
#include <math.h>
#include <deque>
#include "terrain.h"
#include "vec.h"
#include "perlin.h"
#include "image.h"

static bool compareHeight(Vector3 u, Vector3 v)
{
	return (u.y > v.y);
}

/* Terrain2D */
Terrain2D::Terrain2D(int nx, int ny, Vector2 bottomLeft, Vector2 topRight)
	: nx(nx), ny(ny), bottomLeft(bottomLeft), topRight(topRight)
{
	heightField = ScalarField2D(nx, ny, bottomLeft, topRight);
	normalField = ValueField<Vector3>(nx, ny, bottomLeft, topRight);
}

void Terrain2D::InitFromFile(const char* file, float blackAltitude, float whiteAltitude)
{
	Image heightmap;
	heightmap.ReadImage(file);
	float texelX = 1.0f / (heightmap.Width());
	float texelY = 1.0f / (heightmap.Height());

	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			float u = j / ((float)nx - 1);
			float v = i / ((float)ny - 1);

			int anchorX = u * (heightmap.Width() - 1);
			int anchorY = v * (heightmap.Height() - 1);
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
			heightField.Set(i, j, blackAltitude + value * (whiteAltitude - blackAltitude));
		}
	}

	ComputeNormalField();
}

void Terrain2D::InitFromNoise(int blackAltitude, int whiteAltitude)
{
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			//float v = blackAltitude + Noise::ValueNoise2D(i, j, 14589) * (whiteAltitude - blackAltitude);
			float v = blackAltitude + Perlin::Perlin2DAt(1.0, 2.0, 0.5, 6.0, i, j) * (whiteAltitude - blackAltitude);
			heightField.Set(i, j, v);
		}
	}
	ComputeNormalField();
}

double Terrain2D::Height(const Vector2& p) const
{
	return heightField.GetValueBilinear(p);
}

Vector3 Terrain2D::Normal(int i, int j) const
{
	return normalField.Get(i, j);
}

Vector3 Terrain2D::Vertex(int i, int j) const
{
	double x = bottomLeft.x + j * (topRight.x - bottomLeft.x) / (nx - 1);
	double y = heightField.Get(i, j);
	double z = bottomLeft.y + i * (topRight.y - bottomLeft.y) / (ny - 1);
	return Vector3(x, y, z);
}

Mesh Terrain2D::GetMesh() const
{
	Mesh ret;
	ret.CalculateFromTerrain2D(*this);
	return ret;
}

void Terrain2D::SetHeight(int i, int j, double v)
{
	heightField.Set(i, j, v);
}

float Terrain2D::Lerp(float a, float b, float f)
{
	return (a * (1.0f - f)) + (b * f);
}

void Terrain2D::ComputeNormalField()
{
	for (int i = 0; i < ny - 1; i++)
	{
		for (int j = 0; j < nx - 1; j++)
		{
			Vector3 AB = (Vertex(i + 1, j) - Vertex(i, j));
			Vector3 AC = (Vertex(i + 1, j + 1) - Vertex(i, j));
			Vector3 normal = Normalize(Cross(AB, AC));

			normalField.Set(i, j, normalField.Get(i, j) + normal);
			normalField.Set(i + 1, j, normalField.Get(i + 1, j) + normal);
			normalField.Set(i + 1, j + 1, normalField.Get(i + 1, j + 1) + normal);

			AB = AC;
			AC = (Vertex(i, j + 1) - Vertex(i, j));
			normal = Normalize(Cross(AB, AC));

			normalField.Set(i, j, normalField.Get(i, j) + normal);
			normalField.Set(i + 1, j + 1, normalField.Get(i + 1, j + 1) + normal);
			normalField.Set(i, j + 1, normalField.Get(i, j + 1) + normal);
		}
	}
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
			normalField.Set(i, j, Normalize(normalField.Get(i, j)));
	}
}



int Terrain2D::Distribute(Vector2 p, Vector2* neighbours, float* quantity) const
{
	int i = p.x, j = p.y;

	int counter = 0;
	double slope = 0.0f;
	double maxSlope = 0.0f;
	double pointHeight = p.y;

	for (int k = -1; k <= 1; k++)
	{
		for (int l = -1; l <= 1; l++)
		{
			if (k == 0 || l == 0 || (i + k) < 0 || (i + k) > nx || (j + l) < 0 || (j + l) > ny)
				continue;

			double neighHeight = heightField.Get(i + k, j + l);
			if (pointHeight > neighHeight)
			{
				if (k + l == -1 || k + l == 1)
					slope = pointHeight - neighHeight;
				else
					slope = (pointHeight - neighHeight) / sqrt(2);

				if (slope >= maxSlope)
					maxSlope = slope;

				neighbours[counter] = Vector2(i + k, j + l);
				quantity[counter] = 1.0f;
				counter++;
			}
		}
	}
	return counter;
}

ScalarField2D Terrain2D::Drainage() const
{
	std::deque<Vector3> points;
	for (int i = 0; i < ny - 1; i++)
	{
		for (int j = 0; j < nx - 1; j++)
		{
			points.push_back(Vector3(i, heightField.Get(i, j), j));
		}
	}
	std::sort(points.begin(), points.end(), compareHeight);
	ScalarField2D drainage = ScalarField2D(nx, ny, bottomLeft, topRight, 1.0);

	while (!points.empty())
	{
		Vector3 p = points.front();
		points.pop_front();

		int i = p.x, j = p.z;
		Vector2 neighbours[8];
		float quantity[8];
		int n = Distribute(Vector2(i, j), neighbours, quantity);

		for (int k = 0; k < n; k++)
		{
			int l = neighbours[k].x, m = neighbours[k].y;
			drainage.Set(i, j, drainage.Get(i, j) + quantity[k]);
		}
	}
	return drainage;
}

ScalarField2D Terrain2D::WetnessField() const
{
	ScalarField2D drainageField = Drainage();
	ScalarField2D wetnessField = ScalarField2D(nx, ny, bottomLeft, topRight);
	for (int i = 0; i < ny - 1; i++)
	{
		for (int j = 0; j < nx - 1; j++)
		{
			wetnessField.Set(i, j, log(drainageField.Get(i, j) / (1 + (1.0 - normalField.Get(i, j).y))));
		}
	}
	return wetnessField;
}

ScalarField2D Terrain2D::StreamPowerField() const
{
	return ScalarField2D();
}

ScalarField2D Terrain2D::AccessibilityField() const
{
	return ScalarField2D();
}

/* LayerField */
LayerTerrain2D::LayerTerrain2D(int nx, int ny, Vector2 a, Vector2 b)
	: nx(nx), ny(ny), a(a), b(b)
{
	sand = ScalarField2D(nx, ny, a, b);
	bedrock = ScalarField2D(nx, ny, a, b);
}

double LayerTerrain2D::Height(int i, int j) const
{
	return BeckrockValue(i, j) + SandValue(i, j);
}

double LayerTerrain2D::BeckrockValue(int i, int j) const
{
	return bedrock.Get(i, j);
}

double LayerTerrain2D::SandValue(int i, int j) const
{
	return sand.Get(i, j);
}

void LayerTerrain2D::ThermalErosion(int stepCount)
{
	for (int i = 0; i < stepCount; i++)
	{

	}
}

Mesh LayerTerrain2D::GetMesh() const
{
	Terrain2D terrain = Terrain2D(nx, ny, a, b);
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
			terrain.SetHeight(i, j, Height(i, j));
	}
	Mesh m;
	m.CalculateFromTerrain2D(terrain);
	return m;
}