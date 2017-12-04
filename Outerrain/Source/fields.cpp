#include "fields.h"
#include "vec.h"
#include "image.h"
#include "image_io.h"
#include <algorithm>

/* Scalarfield 2D */
Scalarfield2D::Scalarfield2D(int nx, int ny, Vector2 bottomLeft, Vector2 topRight) : nx(nx), ny(ny), bottomLeft(bottomLeft), topRight(topRight)
{
	values.resize(nx * ny);
	std::fill(values.begin(), values.end(), 0);
}

int Scalarfield2D::Index(int row, int column) const
{
	return row * (nx - 1) + column;
}

double Scalarfield2D::At(int i, int j) const
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
Heightfield::Heightfield(int nx, int ny, Vector2 bottomLeft, Vector2 topRight) : Scalarfield2D(nx, ny, bottomLeft, topRight)
{
}

void Heightfield::InitFromFile(const char* file, float blackAltitude, float whiteAltitude)
{
	Image heightmap = read_image(file);

	float texelX = 1.0f / (heightmap.width());
	float texelY = 1.0f / (heightmap.height());

	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			float u = j / ((float)nx - 1);
			float v = i / ((float)ny - 1);

			int anchorX = u * (heightmap.width() - 1);
			int anchorY = v * (heightmap.height() - 1);

			if (anchorX == heightmap.width() - 1)
				anchorX--;
			if (anchorY == heightmap.height() - 1)
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

			values[Index(i, j)] = blackAltitude + value * (whiteAltitude - blackAltitude);
		}
	}
}

void Heightfield::InitFromNoise()
{
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
		{
			values[Index(i, j)] = 1.0;
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
	double x = bottomLeft.x + i*(topRight.x - bottomLeft.x) / (nx - 1);
	double y = At(i, j);
	double z = bottomLeft.y + j* (topRight.y - bottomLeft.y) / (ny - 1);
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

float Heightfield::Lerp(float a, float b, float f)
{
	return (a * (1.0f - f)) + (b * f);
}