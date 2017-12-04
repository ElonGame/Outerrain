#include <algorithm>
#include "fields.h"
#include "vec.h"
#include "perlin.h"

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

void Heightfield::InitFromFile()
{

}

void Heightfield::InitFromNoise(int blackAltitude, int whiteAltitude)
{
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
		{
			values[Index(i, j)] = blackAltitude + Noise::ValueNoise2D(i, j, 14589) * (whiteAltitude - blackAltitude);
			//values[Index(i, j)] = Perlin::Perlin2DAt(1.0, 2.0, 0.5, 6.0, i, j);
		}
	}
}

double Heightfield::Height(const Vector2& p) const
{
	return GetValueBilinear(p);
}

Vector3 Heightfield::Normal(int i, int j) const
{
	// @Todo
	// @Loooong
	return Vector3(0.0);
}

Vector3 Heightfield::Vertex(int i, int j) const
{
	double x = a.x + i*(b.x - a.x) / (nx - 1);
	double y = At(i, j);
	double z = a.y + j* (b.y - a.y) / (ny - 1);
	return Vector3(x, y, z);
}

Mesh Heightfield::GetMesh() const
{
	Mesh ret;

	// Vertices & Texcoords
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
		{
			float u = j / ((float)nx - 1);
			float v = i / ((float)ny - 1);
			ret.AddTexcoord(Vector2(u, 1 - v));
			ret.AddVertex(Vertex(i, j));
		}
	}

	// Normals
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
			ret.AddNormal(Normal(i, j));
	}

	// Triangles
	int verticesPerLine = nx;
	int nbTris = 2 * ((verticesPerLine - 1) * (verticesPerLine - 1));
	int c = 0;
	int vertexArrayLength = verticesPerLine * verticesPerLine;
	while (c < vertexArrayLength - verticesPerLine - 1)
	{
		if (c == 0 || (((c + 1) % verticesPerLine != 0) && c <= vertexArrayLength - verticesPerLine))
		{
			ret.AddTriangle(c, c + verticesPerLine + 1, c + verticesPerLine);
			ret.AddTriangle(c + verticesPerLine + 1, c, c + 1);
		}
		c++;
	}

	return ret;
}