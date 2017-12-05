#include <algorithm>
#include "fields.h"
#include "vec.h"
#include "perlin.h"
#include "image.h"

/* Scalarfield 2D */
Scalarfield2D::Scalarfield2D(int nx, int ny, Vector2 bottomLeft, Vector2 topRight) 
	: nx(nx), ny(ny), bottomLeft(bottomLeft), topRight(topRight)
{
	values.resize(nx * ny);
	std::fill(values.begin(), values.end(), 0);
}

int Scalarfield2D::Index(int row, int column) const
{
	return row * (nx - 1) + column;
}

void Scalarfield2D::Set(int i, int j, double v)
{
	values[Index(i, j)] = v;
}

double Scalarfield2D::Get(int i, int j) const
{
	int index = Index(i, j);
	return values[index];
}

double Scalarfield2D::GetValueBilinear(const Vector2& p) const
{
	Vector2 q = p - bottomLeft;
	Vector2 d = topRight - bottomLeft;

	double u = q[0] / d[0];
	double v = q[1] / d[1];

	int i = int(u * (nx - 1));
	int j = int(v * (ny - 1));

	double v1 = Get(i, j);
	double v2 = Get(i + 1, j);
	double v3 = Get(i + 1, j + 1);
	double v4 = Get(i, j + 1);

	return (1 - u) * (1 - v) * v1
		+ (1 - u) * v * v2
		+ u * (1 - v) * v3
		+ u * v * v4;
}


/* Vector3field 2D*/
Vector3field2D::Vector3field2D(int nx, int ny, Vector2 bottomLeft, Vector2 topRight) 
	: nx(nx), ny(ny), bottomLeft(bottomLeft), topRight(topRight)
{
	values.resize(nx * ny);
	std::fill(values.begin(), values.end(), Vector3(0));
}

int Vector3field2D::Index(int row, int column) const
{
	return row * (nx - 1) + column;
}

Vector3 Vector3field2D::Get(int i, int j) const
{
	int index = Index(i, j);
	return values[index];
}

void Vector3field2D::Set(int i, int j, Vector3 v)
{
	int index = Index(i, j);
	values[index] = v;
}

Vector3 Vector3field2D::GetValueBilinear(const Vector2& p) const
{
	Vector2 q = p - bottomLeft;
	Vector2 d = topRight - bottomLeft;
	float u = q[0] / d[0];
	float v = q[1] / d[1];
	int i = int(u * (nx - 1));
	int j = int(v * (ny - 1));
	Vector3 v1 = Get(i, j);
	Vector3 v2 = Get(i + 1, j);
	Vector3 v3 = Get(i + 1, j + 1);
	Vector3 v4 = Get(i, j + 1);
	return v1 * (1 - u) * (1 - v)
		+ v2 * (1 - u) * v
		+ v3 * u * (1 - v)
		+ v4 * u * v;
}


/* Terrain2D */
Terrain2D::Terrain2D(int nx, int ny, Vector2 bottomLeft, Vector2 topRight)
	: nx(nx), ny(ny), bottomLeft(bottomLeft), topRight(topRight)
{
	heightField = Scalarfield2D(nx, ny, bottomLeft, topRight);
	normalField = Vector3field2D(nx, ny, bottomLeft, topRight);
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
			float v = blackAltitude + Noise::ValueNoise2D(i, j, 14589) * (whiteAltitude - blackAltitude);
			heightField.Set(i, j, v);
			//values[Index(i, j)] = Perlin::Perlin2DAt(1.0, 2.0, 0.5, 6.0, i, j);
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
			Vector3 normal = Cross(AB, AC);

			normalField.Set(i, j, normalField.Get(i, j) + normal);
			normalField.Set(i + 1, j, normalField.Get(i + 1, j) + normal);
			normalField.Set(i + 1, j + 1, normalField.Get(i + 1, j + 1) + normal);
			
			AB = AC;
			AC = (Vertex(i, j + 1) - Vertex(i, j));
			normal = Cross(AB, AC);

			normalField.Set(i, j, normalField.Get(i, j) + normal);
			normalField.Set(i + 1, j + 1, normalField.Get(i + 1, j + 1) + normal);
			normalField.Set(i, j + 1, normalField.Get(i, j + 1) + normal);
		}
	}
	for (int i = 0; i < ny - 1; i++)
	{
		for (int j = 0; j < nx - 1; j++)
			normalField.Set(i, j, Normalize(normalField.Get(i, j)));
	}
}


/* LayerField */
LayerTerrain2D::LayerTerrain2D(int nx, int ny, Vector2 a, Vector2 b) : nx(nx), ny(ny), a(a), b(b)
{
	sand = Scalarfield2D(nx, ny, a, b);
	bedrock = Scalarfield2D(nx, ny, a, b);
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
	/*Scalarfield2D fullTerrain(nx, ny, a, b);
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
			fullTerrain.Set(i, j, Height(i, j));
	}
	m.CalculateFromScalarfield(fullTerrain);*/
	Mesh m;
	return m;
}