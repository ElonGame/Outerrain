#include <algorithm>
#include "terrain.h"
#include "vec.h"
#include "perlin.h"
#include "image.h"


/* Terrain2D */
Terrain2D::Terrain2D(int nx, int ny, Vector2 bottomLeft, Vector2 topRight)
	: nx(nx), ny(ny), bottomLeft(bottomLeft), topRight(topRight)
{
	heightField = ValueField<double>(nx, ny, bottomLeft, topRight);
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

	ComputeNormalFieldFromHeightField();
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
	ComputeNormalFieldFromHeightField();
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

void Terrain2D::ComputeNormalFieldFromHeightField()
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


/* LayerField */
LayerTerrain2D::LayerTerrain2D(int nx, int ny, Vector2 a, Vector2 b) 
	: nx(nx), ny(ny), a(a), b(b)
{
	sand = ValueField<double>(nx, ny, a, b);
	bedrock = ValueField<double>(nx, ny, a, b);
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
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
			terrain.SetHeight(i, j, Height(i, j));
	}
	Mesh m;
	m.CalculateFromTerrain2D(terrain);
	return m;
}